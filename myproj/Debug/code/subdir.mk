################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/mycamera.c \
../code/mycv.c \
../code/otsu.c \
../code/pid.c \
../code/slave.c \
../code/speed_control.c 

COMPILED_SRCS += \
./code/mycamera.src \
./code/mycv.src \
./code/otsu.src \
./code/pid.src \
./code/slave.src \
./code/speed_control.src 

C_DEPS += \
./code/mycamera.d \
./code/mycv.d \
./code/otsu.d \
./code/pid.d \
./code/slave.d \
./code/speed_control.d 

OBJS += \
./code/mycamera.o \
./code/mycv.o \
./code/otsu.o \
./code/pid.o \
./code/slave.o \
./code/speed_control.o 


# Each subdirectory must supply rules for building sources it contributes
code/%.src: ../code/%.c code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/AURIX-v1.9.20-workspace/myproj/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

code/%.o: ./code/%.src code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-code

clean-code:
	-$(RM) ./code/mycamera.d ./code/mycamera.o ./code/mycamera.src ./code/mycv.d ./code/mycv.o ./code/mycv.src ./code/otsu.d ./code/otsu.o ./code/otsu.src ./code/pid.d ./code/pid.o ./code/pid.src ./code/slave.d ./code/slave.o ./code/slave.src ./code/speed_control.d ./code/speed_control.o ./code/speed_control.src

.PHONY: clean-code

