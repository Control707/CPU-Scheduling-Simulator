# Makefile for CPU Scheduling Simulator
# CS 3502 - Operating Systems Project 2

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = cpu_scheduler
SOURCE = cpu_scheduler.c

# Default target
all: $(TARGET)

# Compile the program
$(TARGET): $(SOURCE)
	@echo "Compiling CPU Scheduling Simulator..."
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)
	@echo "✓ Build successful!"
	@echo "Run with: ./$(TARGET)"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run comprehensive tests automatically
test: $(TARGET)
	@echo "Running comprehensive tests..."
	@echo "9" | ./$(TARGET)
	@echo "✓ Tests complete! Check scheduling_results.txt"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(TARGET) *.o
	@echo "✓ Clean complete!"

# Clean everything including results
cleanall: clean
	@echo "Removing all output files..."
	rm -f scheduling_results.txt algorithm_comparison.csv
	@echo "✓ All files cleaned!"

# Show help
help:
	@echo "CPU Scheduling Simulator - Build Commands"
	@echo "=========================================="
	@echo "make          - Compile the program"
	@echo "make run      - Compile and run"
	@echo "make test     - Run comprehensive tests"
	@echo "make clean    - Remove compiled files"
	@echo "make cleanall - Remove all output files"
	@echo "make help     - Show this help message"

.PHONY: all run test clean cleanall help