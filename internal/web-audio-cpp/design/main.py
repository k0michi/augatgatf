import numpy as np
from scipy import signal

def design_upsampler(num_taps, factor):
    taps = signal.firwin(num_taps, cutoff=1/factor, window=('kaiser', 14.0))
    taps *= factor
    return taps

def design_downsampler(num_taps, factor):
    taps = signal.firwin(num_taps, cutoff=1/factor, window=('kaiser', 14.0))
    return taps

def to_c_float(number):
    return format(number, '.17g') + "f"

def to_c_float_array(data, name):
    c_array = f"const float {name}[] = {{\n"
    for i, value in enumerate(data):
        c_array += f"{to_c_float(value)}, "
    c_array += "\n};\n"
    return c_array

print(to_c_float_array(design_upsampler(128, 2), "upsampler_2x"))
print(to_c_float_array(design_downsampler(128, 2), "downsampler_2x"))