from PIL import Image
from PyPDF2 import PdfReader
import os

def extract_metadata(file_path, app):
    app.log(f"[*] Extracting metadata from: {file_path}")
    try:
        _, extension = os.path.splitext(file_path)

        if extension.lower() in ['.jpg', '.jpeg', '.png', '.gif']:
            with Image.open(file_path) as img:
                exif_data = img._getexif()
                if exif_data:
                    app.log("[+] Image Metadata (EXIF):")
                    for tag, value in exif_data.items():
                        app.log(f"  - {tag}: {value}")
                else:
                    app.log("[-] No EXIF metadata found.")

        elif extension.lower() == '.pdf':
            with open(file_path, 'rb') as f:
                reader = PdfReader(f)
                info = reader.metadata
                if info:
                    app.log("[+] PDF Metadata:")
                    for key, value in info.items():
                        app.log(f"  - {key}: {value}")
                else:
                    app.log("[-] No metadata found in PDF.")

        else:
            app.log("[!] Unsupported file type for metadata extraction.")

    except Exception as e:
        app.log(f"[!] An error occurred: {e}")
