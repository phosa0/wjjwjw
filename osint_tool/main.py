import customtkinter as ctk
from tkinter import filedialog, messagebox
from modules import username_scanner, ip_tracker, email_breach_checker, phone_lookup, metadata_extractor, domain_scanner
import threading

class OSINTTool(ctk.CTk):
    def __init__(self):
        super().__init__()

        self.title("All-in-One OSINT Tool")
        self.geometry("900x700")
        ctk.set_appearance_mode("Dark")
        ctk.set_default_color_theme("blue")

        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=1)

        self.create_widgets()

    def create_widgets(self):
        # Main frame
        main_frame = ctk.CTkFrame(self)
        main_frame.grid(row=0, column=0, padx=10, pady=10, sticky="nsew")
        main_frame.grid_columnconfigure(0, weight=1)

        # Tab view
        self.tab_view = ctk.CTkTabview(main_frame, anchor="w")
        self.tab_view.pack(fill="x", expand=True)

        self.tab_view.add("Username")
        self.tab_view.add("IP Tracker")
        self.tab_view.add("Email Breach")
        self.tab_view.add("Phone Lookup")
        self.tab_view.add("Metadata")
        self.tab_view.add("Domain")

        # Log window
        log_frame = ctk.CTkFrame(self)
        log_frame.grid(row=1, column=0, padx=10, pady=(0, 10), sticky="nsew")
        log_frame.grid_columnconfigure(0, weight=1)
        log_frame.grid_rowconfigure(0, weight=1)

        self.log_window = ctk.CTkTextbox(log_frame, state="disabled", wrap="word", font=("Consolas", 12))
        self.log_window.grid(row=0, column=0, sticky="nsew")

        # Control buttons
        controls_frame = ctk.CTkFrame(main_frame)
        controls_frame.pack(fill="x", pady=5)

        clear_button = ctk.CTkButton(controls_frame, text="Clear Log", command=self.clear_log)
        clear_button.pack(side="left", padx=5)

        self.theme_menu = ctk.CTkOptionMenu(controls_frame, values=["Dark", "Light"], command=self.change_theme)
        self.theme_menu.pack(side="right", padx=5)
        self.theme_menu.set("Dark")

        # Add widgets to each tab
        self.create_username_scanner_tab()
        self.create_ip_tracker_tab()
        self.create_email_breach_checker_tab()
        self.create_phone_lookup_tab()
        self.create_metadata_extractor_tab()
        self.create_domain_scanner_tab()

    def create_username_scanner_tab(self):
        tab = self.tab_view.tab("Username")
        frame = ctk.CTkFrame(tab)
        frame.pack(fill="x", padx=5, pady=5)

        label = ctk.CTkLabel(frame, text="Username:")
        label.pack(side="left", padx=(5, 0))

        self.username_entry = ctk.CTkEntry(frame, placeholder_text="Enter username")
        self.username_entry.pack(side="left", fill="x", expand=True, padx=5)

        button = ctk.CTkButton(frame, text="Scan", command=self.run_username_scan)
        button.pack(side="left", padx=(0, 5))

    def create_ip_tracker_tab(self):
        tab = self.tab_view.tab("IP Tracker")
        frame = ctk.CTkFrame(tab)
        frame.pack(fill="x", padx=5, pady=5)

        label = ctk.CTkLabel(frame, text="IP Address:")
        label.pack(side="left", padx=(5, 0))

        self.ip_entry = ctk.CTkEntry(frame, placeholder_text="Enter IP address")
        self.ip_entry.pack(side="left", fill="x", expand=True, padx=5)

        self.ipinfo_api_key_entry = ctk.CTkEntry(frame, placeholder_text="IPinfo API Key (Optional)")
        self.ipinfo_api_key_entry.pack(side="left", fill="x", expand=True, padx=5)

        button = ctk.CTkButton(frame, text="Track", command=self.run_ip_tracking)
        button.pack(side="left", padx=(0, 5))

    def create_email_breach_checker_tab(self):
        tab = self.tab_view.tab("Email Breach")
        frame = ctk.CTkFrame(tab)
        frame.pack(fill="x", padx=5, pady=5)

        label = ctk.CTkLabel(frame, text="Email:")
        label.pack(side="left", padx=(5, 0))

        self.email_entry = ctk.CTkEntry(frame, placeholder_text="Enter email address")
        self.email_entry.pack(side="left", fill="x", expand=True, padx=5)

        self.hibp_api_key_entry = ctk.CTkEntry(frame, placeholder_text="HIBP API Key")
        self.hibp_api_key_entry.pack(side="left", fill="x", expand=True, padx=5)

        button = ctk.CTkButton(frame, text="Check", command=self.run_email_breach_check)
        button.pack(side="left", padx=(0, 5))

    def create_phone_lookup_tab(self):
        tab = self.tab_view.tab("Phone Lookup")
        frame = ctk.CTkFrame(tab)
        frame.pack(fill="x", padx=5, pady=5)

        label = ctk.CTkLabel(frame, text="Phone Number:")
        label.pack(side="left", padx=(5, 0))

        self.phone_entry = ctk.CTkEntry(frame, placeholder_text="Enter phone number (with country code)")
        self.phone_entry.pack(side="left", fill="x", expand=True, padx=5)

        self.numverify_api_key_entry = ctk.CTkEntry(frame, placeholder_text="Numverify API Key")
        self.numverify_api_key_entry.pack(side="left", fill="x", expand=True, padx=5)

        button = ctk.CTkButton(frame, text="Lookup", command=self.run_phone_lookup)
        button.pack(side="left", padx=(0, 5))

    def create_metadata_extractor_tab(self):
        tab = self.tab_view.tab("Metadata")
        frame = ctk.CTkFrame(tab)
        frame.pack(fill="x", padx=5, pady=5)

        self.file_path_label = ctk.CTkLabel(frame, text="No file selected.")
        self.file_path_label.pack(side="left", padx=(5, 0), fill="x", expand=True)

        browse_button = ctk.CTkButton(frame, text="Browse", command=self.browse_file)
        browse_button.pack(side="left", padx=5)

        extract_button = ctk.CTkButton(frame, text="Extract", command=self.run_metadata_extraction)
        extract_button.pack(side="left", padx=(0, 5))

    def create_domain_scanner_tab(self):
        tab = self.tab_view.tab("Domain")
        frame = ctk.CTkFrame(tab)
        frame.pack(fill="x", padx=5, pady=5)

        label = ctk.CTkLabel(frame, text="Domain:")
        label.pack(side="left", padx=(5, 0))

        self.domain_entry = ctk.CTkEntry(frame, placeholder_text="Enter domain (e.g., example.com)")
        self.domain_entry.pack(side="left", fill="x", expand=True, padx=5)

        button = ctk.CTkButton(frame, text="Scan", command=self.run_domain_scan)
        button.pack(side="left", padx=(0, 5))

    def log(self, message):
        def _log():
            self.log_window.configure(state="normal")
            self.log_window.insert("end", message + "\n")
            self.log_window.configure(state="disabled")
            self.log_window.see("end")
        self.after(0, _log)

    def clear_log(self):
        self.log_window.configure(state="normal")
        self.log_window.delete("1.0", "end")
        self.log_window.configure(state="disabled")

    def change_theme(self, new_theme):
        ctk.set_appearance_mode(new_theme)

    def browse_file(self):
        self.selected_file = filedialog.askopenfilename()
        if self.selected_file:
            self.file_path_label.configure(text=self.selected_file)

    def start_threaded_task(self, target, *args):
        thread = threading.Thread(target=target, args=args)
        thread.daemon = True
        thread.start()

    def run_username_scan(self):
        username = self.username_entry.get()
        if username:
            self.start_threaded_task(username_scanner.scan_username, username, self)
        else:
            messagebox.showwarning("Input Error", "Please enter a username.")

    def run_ip_tracking(self):
        ip = self.ip_entry.get()
        api_key = self.ipinfo_api_key_entry.get()
        if ip:
            self.start_threaded_task(ip_tracker.track_ip, ip, self, api_key)
        else:
            messagebox.showwarning("Input Error", "Please enter an IP address.")

    def run_email_breach_check(self):
        email = self.email_entry.get()
        api_key = self.hibp_api_key_entry.get()
        if email and api_key:
            self.start_threaded_task(email_breach_checker.check_email_breach, email, self, api_key)
        else:
            messagebox.showwarning("Input Error", "Please enter both email and HIBP API key.")

    def run_phone_lookup(self):
        phone = self.phone_entry.get()
        api_key = self.numverify_api_key_entry.get()
        if phone and api_key:
            self.start_threaded_task(phone_lookup.lookup_phone_number, phone, self, api_key)
        else:
            messagebox.showwarning("Input Error", "Please enter both phone number and Numverify API key.")

    def run_metadata_extraction(self):
        if hasattr(self, 'selected_file') and self.selected_file:
            self.start_threaded_task(metadata_extractor.extract_metadata, self.selected_file, self)
        else:
            messagebox.showwarning("Input Error", "Please select a file first.")

    def run_domain_scan(self):
        domain = self.domain_entry.get()
        if domain:
            self.start_threaded_task(domain_scanner.scan_domain, domain, self)
        else:
            messagebox.showwarning("Input Error", "Please enter a domain.")

if __name__ == "__main__":
    app = OSINTTool()
    app.mainloop()
