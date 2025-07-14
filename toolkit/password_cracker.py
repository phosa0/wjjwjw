import hashlib

def crack_password(password_hash, wordlist):
    for word in wordlist:
        guess = hashlib.sha256(word.encode('utf-8')).hexdigest()
        if guess == password_hash:
            return word
    return None
