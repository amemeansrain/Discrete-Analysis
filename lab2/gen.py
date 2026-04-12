import random
import string
import sys

MAX_KEY_LEN = 256
MAX_VAL = 2**64 - 1

def gen_word():
    length = random.randint(1, MAX_KEY_LEN)
    return ''.join(random.choice(string.ascii_letters) for _ in range(length))

def run_generator(count):
    test_dict = {}
    commands = []
    answers = []

    for _ in range(count):
        choice = random.random()
        
        if choice < 0.4:  # +
            word = gen_word()
            val = random.randint(0, MAX_VAL)
            word_low = word.lower()
            
            commands.append(f"+ {word} {val}")
            if word_low in test_dict:
                answers.append("Exist")
            else:
                test_dict[word_low] = val
                answers.append("OK")
                
        elif choice < 0.7:  # find
            word = gen_word()
            word_low = word.lower()
            
            commands.append(word)
            if word_low in test_dict:
                answers.append(f"OK: {test_dict[word_low]}")
            else:
                answers.append("NoSuchWord")
                
        else:  # -
            word = gen_word()
            word_low = word.lower()
            
            commands.append(f"- {word}")
            if word_low in test_dict:
                del test_dict[word_low]
                answers.append("OK")
            else:
                answers.append("NoSuchWord")

    with open("input.txt", "w") as f_in, open("expected.txt", "w") as f_ans:
        f_in.write("\n".join(commands) + "\n")
        f_ans.write("\n".join(answers) + "\n")

if __name__ == "__main__":
    n = 100000
    if len(sys.argv) > 1:
        n = int(sys.argv[1])

    run_generator(n)
    print(f"Generated {n} tests")