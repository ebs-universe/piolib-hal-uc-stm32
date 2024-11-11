Import('env')
from os.path import join, realpath

# Private flags for the current "hal-stm32" source files
defines = env.get("CPPDEFINES", [])

if "STM32U0" in defines:
    print("Using ststm32 u0xx HAL implementation")
    env.Replace(SRC_FILTER=["+<hal_platform/common/*.*>", "+<hal_platform/u0xx/*.*>"])

elif "STM32F4" in defines:
    print("Using ststm32 f4xx HAL implementation")
    env.Replace(SRC_FILTER=["+<hal_platform/common/*.*>", "+<hal_platform/f4xx/*.*>"])

elif "STM32H7RS" in defines:
    print("Using ststm32 h7rsxx HAL implementation")
    env.Replace(SRC_FILTER=["+<hal_platform/common/*.*>", "+<hal_platform/h7rsxx/*.*>"])

else:
    raise ValueError("Unsupported STM32 Family.")