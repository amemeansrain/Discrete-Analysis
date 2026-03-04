import random
import string
import sys

def generate(count):
    letters = string.ascii_uppercase
    for _ in range(count):
        plate = f"{random.choice(letters)} {random.randint(0, 999):03d} {random.choice(letters)}{random.choice(letters)}"
        val = ''.join(random.choices(string.ascii_letters, k=random.randint(1, 2048)))
        print(f"{plate}\t{val}")

if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 100
    generate(n)