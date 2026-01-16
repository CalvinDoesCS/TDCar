# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I./include `pkg-config --cflags opencv4`
LDFLAGS = -lwiringPi -lpthread -lm -lncurses `pkg-config --libs opencv4`

# Directories
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = test

# Source files
SOURCES = $(SRC_DIR)/motor.cpp $(SRC_DIR)/robot_hat_pwm.cpp $(SRC_DIR)/servo.cpp $(SRC_DIR)/picarx.cpp $(SRC_DIR)/keyboard_control.cpp $(SRC_DIR)/pid.cpp $(SRC_DIR)/ultrasonic.cpp $(SRC_DIR)/camera.cpp
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Targets
all: $(BUILD_DIR)/picar_drive $(BUILD_DIR)/motor_class_test $(BUILD_DIR)/camera_test $(BUILD_DIR)/camera_web_server

# Main program
$(BUILD_DIR)/picar_drive: $(OBJECTS) $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Test program
$(BUILD_DIR)/motor_class_test: $(OBJECTS) $(TEST_DIR)/motor_class_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Camera test program
$(BUILD_DIR)/camera_test: $(BUILD_DIR)/camera.o $(SRC_DIR)/camera_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Camera web server
$(BUILD_DIR)/camera_web_server: $(BUILD_DIR)/camera.o $(SRC_DIR)/camera_web_server.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/picar_drive $(BUILD_DIR)/motor_class_test $(BUILD_DIR)/camera_test $(BUILD_DIR)/camera_web_server

# Run main program
run: $(BUILD_DIR)/picar_drive
	sudo ./$(BUILD_DIR)/picar_drive

# Run tests
test: $(BUILD_DIR)/motor_class_test
	sudo ./$(BUILD_DIR)/motor_class_test

# Run camera test
camera: $(BUILD_DIR)/camera_test
	./$(BUILD_DIR)/camera_test

# Run camera web server
camera_web: $(BUILD_DIR)/camera_web_server
	./$(BUILD_DIR)/camera_web_server

.PHONY: all clean run test camera camera_web
