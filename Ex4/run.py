import subprocess
import time
import os
import re
import sys

# Define variables to store the output values
time_ms_list = []
data_sent_list = []
data_rate_list = []
throughput_list = []
ip_address = "192.168.1.41"
# ip_address = "192.168.226.212"
if sys.argv[1] == "f":
    serv = "./udp_servf"
    client = "./udp_clientf"
else:
    serv = "./udp_serv4"
    client = "./udp_client4"
cwd = os.path.dirname(os.path.abspath(__file__))
# Run the C codes three times
print(f"Running: \n Server: {serv}\n Client: {client}")
for i in range(10):
    subprocess.Popen(["gnome-terminal", "--", serv], cwd=cwd)
    time.sleep(1)
    # Run the first C code in a separate terminal window
    output = subprocess.check_output([client, ip_address])
    output_str = output.decode("utf-8").split("\n")
    # Read the stdout of the second C code line by line
    for line in iter(output_str):
        # Check if the line contains the desired output
        if "Time(ms)" in line:
            time_match = re.search(r"Time\(ms\) : (\d+.\d+),", line)
            time_ms = float(time_match.group(1))
            time_ms_list.append(float(time_ms))
        if "Throughput:" in line:
            throughput_match = re.search(r" Throughput: (\d+.\d+) \(Mbps\)", line)
            throughput = float(throughput_match.group(1))
            throughput_list.append(float(throughput))
    time.sleep(2)
    # os.system("bash -c 'read -s -n 1 -p \"Press any key to continue run\n\"'")

# Calculate the average of the output values
time_ms_avg = sum(time_ms_list) / len(time_ms_list)
throughput_avg = sum(throughput_list) / len(throughput_list)

# Print the average values
print("Time(ms) Average:", time_ms_avg)
print("Throughput(Mbps) Average:", throughput_avg)

os.system("bash -c 'read -s -n 1 -p'")
