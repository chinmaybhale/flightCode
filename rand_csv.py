import random

sensor_list = [0, 0, 0, 0, 0, 0, 0, 0, 0]

for i in range(0, 100):
    for j in range(0, 9):
        sensor_list[j] = random.uniform(0, 1000)

    print(f"{sensor_list[0]},{sensor_list[1]},{sensor_list[2]},{sensor_list[3]},{sensor_list[4]},{sensor_list[5]},{sensor_list[6]},{sensor_list[7]},{sensor_list[8]}")
