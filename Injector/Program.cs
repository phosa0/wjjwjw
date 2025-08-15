using System;
using System.Windows.Forms;
using System.Drawing;

namespace Injector
{
    public class MainForm : Form
    {
        private TextBox processNameTextBox;
        private Label dllPathLabel;
        private Button browseButton;
        private Button injectButton;
        private Label processLabel;
        private Label dllLabel;
        private string selectedDllPath;

        public MainForm()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            this.Text = "PhasmoTrainer Injector";
            this.Size = new Size(420, 200);
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;

            // Process Label
            processLabel = new Label();
            processLabel.Text = "Process Name:";
            processLabel.Location = new Point(12, 15);
            processLabel.Size = new Size(100, 20);
            this.Controls.Add(processLabel);

            // Process Name TextBox
            processNameTextBox = new TextBox();
            processNameTextBox.Text = "Phasmophobia";
            processNameTextBox.Location = new Point(110, 12);
            processNameTextBox.Size = new Size(180, 20);
            this.Controls.Add(processNameTextBox);

            // Browse Button
            browseButton = new Button();
            browseButton.Text = "Browse DLL";
            browseButton.Location = new Point(300, 10);
            browseButton.Size = new Size(94, 23);
            browseButton.Click += new EventHandler(this.browseButton_Click);
            this.Controls.Add(browseButton);

            // DLL Label
            dllLabel = new Label();
            dllLabel.Text = "DLL Path:";
            dllLabel.Location = new Point(12, 55);
            dllLabel.Size = new Size(100, 20);
            this.Controls.Add(dllLabel);

            // DLL Path Label
            dllPathLabel = new Label();
            dllPathLabel.Text = "No DLL Selected";
            dllPathLabel.Location = new Point(110, 55);
            dllPathLabel.Size = new Size(284, 23);
            dllPathLabel.BorderStyle = BorderStyle.Fixed3D;
            dllPathLabel.AutoEllipsis = true;
            this.Controls.Add(dllPathLabel);

            // Inject Button
            injectButton = new Button();
            injectButton.Text = "Inject";
            injectButton.Location = new Point(150, 100);
            injectButton.Size = new Size(120, 40);
            injectButton.Click += new EventHandler(this.injectButton_Click);
            injectButton.Enabled = false;
            this.Controls.Add(injectButton);
        }

        private void browseButton_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.Filter = "DLL Files (*.dll)|*.dll";
                ofd.Title = "Select a DLL File";
                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    selectedDllPath = ofd.FileName;
                    dllPathLabel.Text = selectedDllPath;
                    injectButton.Enabled = true;
                }
            }
        }

        private void injectButton_Click(object sender, EventArgs e)
        {
            string processName = processNameTextBox.Text;
            if (string.IsNullOrWhiteSpace(processName))
            {
                MessageBox.Show("Please enter a process name.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            System.Diagnostics.Process[] processes = System.Diagnostics.Process.GetProcessesByName(processName.Replace(".exe", ""));
            if (processes.Length == 0)
            {
                MessageBox.Show($"Process '{processName}' not found.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            try
            {
                Inject(processes[0].Id, selectedDllPath);
                MessageBox.Show("DLL injected successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to inject DLL: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        #region DLL Injection Logic

        // P/Invoke signatures for Win32 API functions
        [System.Runtime.InteropServices.DllImport("kernel32.dll")]
        private static extern IntPtr OpenProcess(int dwDesiredAccess, bool bInheritHandle, int dwProcessId);

        [System.Runtime.InteropServices.DllImport("kernel32.dll", CharSet = System.Runtime.InteropServices.CharSet.Auto)]
        private static extern IntPtr GetModuleHandle(string lpModuleName);

        [System.Runtime.InteropServices.DllImport("kernel32.dll", CharSet = System.Runtime.InteropServices.CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string procName);

        [System.Runtime.InteropServices.DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
        private static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);

        [System.Runtime.InteropServices.DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint nSize, out UIntPtr lpNumberOfBytesWritten);

        [System.Runtime.InteropServices.DllImport("kernel32.dll")]
        private static extern IntPtr CreateRemoteThread(IntPtr hProcess, IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, IntPtr lpThreadId);

        [System.Runtime.InteropServices.DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool CloseHandle(IntPtr hObject);

        // Constants for API calls
        private const int PROCESS_CREATE_THREAD = 0x0002;
        private const int PROCESS_QUERY_INFORMATION = 0x0400;
        private const int PROCESS_VM_OPERATION = 0x0008;
        private const int PROCESS_VM_WRITE = 0x0020;
        private const int PROCESS_VM_READ = 0x0010;
        private const uint MEM_COMMIT = 0x1000;
        private const uint MEM_RESERVE = 0x2000;
        private const uint PAGE_READWRITE = 0x04;

        private void Inject(int processId, string dllPath)
        {
            IntPtr hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, processId);
            if (hProcess == IntPtr.Zero) throw new Exception("Failed to open process.");

            IntPtr loadLibraryAddr = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
            if (loadLibraryAddr == IntPtr.Zero) throw new Exception("Failed to find LoadLibraryA.");

            uint dllPathSize = (uint)((dllPath.Length + 1) * System.Runtime.InteropServices.Marshal.SizeOf(typeof(char)));
            IntPtr allocMemAddress = VirtualAllocEx(hProcess, IntPtr.Zero, dllPathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (allocMemAddress == IntPtr.Zero) throw new Exception("Failed to allocate memory in process.");

            byte[] dllBytes = System.Text.Encoding.Default.GetBytes(dllPath);
            WriteProcessMemory(hProcess, allocMemAddress, dllBytes, (uint)dllBytes.Length, out UIntPtr bytesWritten);
            if (bytesWritten.ToUInt32() == 0) throw new Exception("Failed to write to process memory.");

            IntPtr hThread = CreateRemoteThread(hProcess, IntPtr.Zero, 0, loadLibraryAddr, allocMemAddress, 0, IntPtr.Zero);
            if (hThread == IntPtr.Zero) throw new Exception("Failed to create remote thread.");

            CloseHandle(hProcess);
        }

        #endregion
    }

    public static class Program
    {
        [STAThread]
        public static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Injector.MainForm());
        }
    }
}
