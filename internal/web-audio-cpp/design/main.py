import numpy as np
from scipy import signal

def design_upsampler(num_taps, factor):
    taps = signal.firwin(num_taps, cutoff=1/factor, window=('kaiser', 14.0))
    taps *= factor
    return taps

def design_downsampler(num_taps, factor):
    taps = signal.firwin(num_taps, cutoff=1/factor, window=('kaiser', 14.0))
    return taps

print(design_upsampler(128, 2))