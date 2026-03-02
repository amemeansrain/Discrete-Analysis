import sys

def check():
    last_key = ""
    line_num = 0
    for line in sys.stdin:
        line_num += 1
        if not line.strip(): continue
        
        # Ключ — это первые 8 символов "A 000 AA"
        current_key = line[:8]
        
        if current_key < last_key:
            print(f"Ошибка на строке {line_num}: {current_key} < {last_key}")
            sys.exit(1)
        last_key = current_key
    
    print(f"Проверка пройдена! Обработано {line_num} строк.")

if __name__ == "__main__":
    check()