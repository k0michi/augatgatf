import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

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

def plot_upsampler_response(taps, factor, title):
    block_size = 64
    t = np.arange(block_size)
    input_wave = np.sin(2 * np.pi * t / block_size)

    upsampled = np.zeros(block_size * factor)
    upsampled[::factor] = input_wave

    output_wave = np.convolve(upsampled, taps, mode='same')

    plt.figure(figsize=(10, 4))
    plt.plot(upsampled, label=f"Zero-stuffed ({factor}x)", marker=".")
    plt.plot(output_wave, label="Filtered Output", marker="o")
    plt.plot(input_wave, label="Original", marker="o")
    plt.title(title)
    plt.legend()
    plt.show(block=False)

def plot_downsampler_response(taps, factor, title):
    block_size = 128
    t = np.arange(block_size)
    input_wave = np.sin(2 * np.pi * t / block_size)

    filtered = np.convolve(input_wave, taps, mode='same')
    downsampled = filtered[::factor]

    plt.figure(figsize=(10, 4))
    plt.plot(input_wave, label="Original", marker="o")
    plt.plot(filtered, label="Filtered", marker=".")
    plt.plot(downsampled, label=f"Downsampled ({factor}x)", marker="o")
    plt.title(title)
    plt.legend()
    plt.show(block=False)

upsampler_2x = design_upsampler(128, 2)
print(to_c_float_array(upsampler_2x, "upsampler_2x"))
plot_upsampler_response(upsampler_2x, 2, "2x Upsampler Filter Response")

downsampler_2x = design_downsampler(128, 2)
print(to_c_float_array(downsampler_2x, "downsampler_2x"))
plot_downsampler_response(downsampler_2x, 2, "2x Downsampler Filter Response")

upsampler_4x = design_upsampler(256, 4)
print(to_c_float_array(upsampler_4x, "upsampler_4x"))
plot_upsampler_response(upsampler_4x, 4, "4x Upsampler Filter Response")

downsampler_4x = design_downsampler(256, 4)
print(to_c_float_array(downsampler_4x, "downsampler_4x"))
plot_downsampler_response(downsampler_4x, 4, "4x Downsampler Filter Response")

plt.show()