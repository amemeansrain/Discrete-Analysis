import sys

def check():
    last_key = ""
    line_num = 0
    
    for line in sys.stdin:
        line = line.strip()
        if not line: continue
        line_num += 1
        
        parts = line.split('\t')    
        current_key = parts[0]
        
        # 1. Проверка порядка (Key_i >= Key_{i-1})
        if current_key < last_key:
            print(f"ОШИБКА ПОРЯДКА на строке {line_num}!")
            print(f"Текущий ключ:  [{current_key}]")
            print(f"Предыдущий:    [{last_key}]")
            sys.exit(1)
            
        last_key = current_key

    print(f"✅ Успех! Проверено строк: {line_num}")

if __name__ == "__main__":
    check()