"""Read a csv and make an audio file"""
import scipy.io.wavfile as wf
import numpy as np
import pandas as pd

RATE = 44100

def my_norm(a_val, your_min, your_max):
    """Normalise array between min and max
    """
    ratio = (your_min - your_max)/(np.max(a_val)-np.min(a_val))
    #as you want your data to be between -1 and 1, everything should be scaled to 2,
    #if your desired min and max are other values, replace 2 with your_max - your_min
    shift = (np.max(a_val)+np.min(a_val))/2
    #now you need to shift the center to the middle, this is not the average of the values.
    return (a_val - shift)*ratio

FILENAME = "./audio.csv"
df = pd.read_csv(FILENAME, header=None, delim_whitespace=False)
data_array = df.to_numpy()

new_data = my_norm(data_array[~np.isnan(data_array)], -1, 1)

wf.write('test.wav', RATE, new_data)
