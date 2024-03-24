import threading
import random
import time

LOWER_NUM = 1
UPPER_NUM = 10000
BUFFER_SIZE = 100
MAX_COUNT = 10000

buffer = []
lock = threading.Lock()
producer_completed = False
customers_completed = False

def producer():
    global producer_completed
    start_time = time.time()
    for _ in range(MAX_COUNT):
        num = random.randint(LOWER_NUM, UPPER_NUM)
        with lock:
            buffer.append(num)
            with open("all.txt", "a") as f:
                f.write(str(num) + "\n")
    producer_completed = True
    end_time = time.time()
    print("Producer runtime:", end_time - start_time, "seconds")

def customer_even():
    global customers_completed
    start_time = time.time()
    while not producer_completed or buffer:
        with lock:
            if buffer and buffer[-1] % 2 == 0:
                num = buffer.pop()
                with open("even.txt", "a") as f:
                    f.write(str(num) + "\n")
        if not buffer and producer_completed:
            break
    customers_completed = True
    end_time = time.time()
    print("Even Customer runtime:", end_time - start_time, "seconds")

def customer_odd():
    global customers_completed
    start_time = time.time()
    while not producer_completed or buffer:
        with lock:
            if buffer and buffer[-1] % 2 != 0:
                num = buffer.pop()
                with open("odd.txt", "a") as f:
                    f.write(str(num) + "\n")
        if not buffer and producer_completed:
            break
    customers_completed = True
    end_time = time.time()
    print("Odd Customer runtime:", end_time - start_time, "seconds")

if __name__ == "__main__":
    start_total_time = time.time()
    producer_thread = threading.Thread(target=producer)
    customer_even_thread = threading.Thread(target=customer_even)
    customer_odd_thread = threading.Thread(target=customer_odd)

    producer_thread.start()
    customer_even_thread.start()
    customer_odd_thread.start()

    producer_thread.join()
    customer_even_thread.join()
    customer_odd_thread.join()

    end_total_time = time.time()
    print("Total runtime:", end_total_time - start_total_time, "seconds")
    print("Process terminated.")
