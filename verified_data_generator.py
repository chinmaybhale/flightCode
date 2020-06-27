import random

sensor_count = 15

base_val = 0
pos_err = 0
neg_err = 0

sensor_str = ""

for i in range(0, 100):
    sensor_str = ""
    for s in range(0, sensor_count - 1):
        base_val = random.uniform(0, 2000)
        pos_err = random.uniform(0, 0.2)
        neg_err = pos_err

        sensor_str += f"{base_val},{pos_err},{neg_err},"

    base_val = random.uniform(0, 2000)
    pos_err = random.uniform(0, 0.2)
    neg_err = pos_err

    sensor_str += f"{base_val},{pos_err},{neg_err}"
    print(sensor_str)
