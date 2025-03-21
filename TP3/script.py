
import os
import numpy as np
import matplotlib.pyplot as plt

def analyze_files():
    files = [f"benchmark/preprocessingn={num}.csv" for num in range(10, 201, 10)]
    means = []
    labels = []
    timeMeans=[]
    
    for file in files:
        if not os.path.exists(file):
            print(f"Warning: {file} not found.")
            continue
        n=0;
        data = []
        time=[]
        with open(file, 'r') as f:
            for line in f:
                parts = line.strip().split(',')
                if len(parts) == 4:
                    try:
                        n=int(parts[0])
                        data.append(float(parts[3])*100)
                        time.append(float(parts[2]))
                    except ValueError:
                        print(f"Skipping malformed line in {file}: {line.strip()}")
        
        if not data:
            print(f"No valid data in {file}.")
            continue
        
        col4_avg = np.mean(data)
        col3_avg=np.mean(time)
        means.append(col4_avg)
        timeMeans.append(col3_avg)
        labels.append(n)
    
    plt.bar(labels, means)
    plt.xlabel("# of objets")
    plt.ylabel("Reduced variable (%)")
    plt.title("Mean of percentage of preprocessed variable")
    plt.xticks(rotation=45)
    plt.show()
    plt.bar(labels,timeMeans)
    plt.ylabel("Time spent for preprocessing(s)")
    plt.title("Time spent for preprocessing variables, depending on the number of those")
    plt.show()



if __name__ == "__main__":

    analyze_files()

