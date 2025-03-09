// Implementation of a Single Layer Perceptron
// AND Gate Implementation
// Uses two pushbuttons as inputs and a pushbutton to trigger training
// Outputs weights and bias on LEDs
// Main Output on LED

// VERSION 01

// Pin definitions
const int input1Pin = 2;      // Pushbutton for X1 (input 1)
const int input2Pin = 3;      // Pushbutton for X2 (input 2)
const int epochButtonPin = 4; // Pushbutton to trigger training

const int outputLed = 5;      // LED for Output
const int biasLed = 6;        // LED for Bias
const int weight1Led = 7;     // LED for Weight 1 (w1)
const int weight2Led = 8;     // LED for Weight 2 (w2);

// Perceptron variables
float w1 = 0.0, w2 = 0.0, bias = 2.0;  // Weights and bias
float learningRate = 0.1;              // Learning rate
// Previous values are tracked for LED Light transitions
float prevW1 = 0.0, prevW2 = 0.0, prevBias = 0.0;
// Epoch count for counting training iterations
int epochcount = 0; 

// AND gate truth table
// Inputs are the binary values of X1 and X2
// Outputs are the expected results of the AND gate
int inputs[4][2] = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
int targets[4] = {0, 0, 0, 1}; // Expected outputs for AND gate

// Stores calculated outputs for each input
int outputValues[4] = {0, 0, 0, 0};

// Setup Function
// Initializes Serial communication and pin modes for inputs and outputs
void setup() {
    Serial.begin(9600);
    pinMode(input1Pin, INPUT);
    pinMode(input2Pin, INPUT);
    pinMode(epochButtonPin, INPUT);

    pinMode(outputLed, OUTPUT);
    pinMode(biasLed, OUTPUT);
    pinMode(weight1Led, OUTPUT);
    pinMode(weight2Led, OUTPUT);

    Serial.println("Perceptron AND gate training ready. Press epoch button to train.");
}

// Main Loop Function
// Checks if the epoch button is pressed to start training 
// checks the perceptron output based on the input pushbuttons.
void loop() {
    if (digitalRead(epochButtonPin) == HIGH) { // Epoch button pressed
        delay(200); // Debounce delay
        Serial.println("\nEpoch Started...");
        epochcount += 1;            // Increment epoch count
        Serial.println("Epoch");    
        Serial.println(epochcount);
        trainPerceptron();        // Train the perceptron
        showTrainingResults();  // Display training results on LEDs
        Serial.println("Epoch Completed! Press button to check output.");
    } else {
        checkOutput(); // Check perceptron output when inputs are pressed
    }

    delay(100); // Slow down loop
}

// Function to train the perceptron
void trainPerceptron() {
    for (int i = 0; i < 4; i++) {
        int x1 = inputs[i][0];
        int x2 = inputs[i][1];
        int target = targets[i];

        // Calculate perceptron output
        float sum = (x1 * w1) + (x2 * w2) + bias;
        int output = (sum >= 0) ? 1 : 0;

        // Update weights and bias
        float error = target - output;
        w1 += learningRate * error * x1;
        w2 += learningRate * error * x2;
        bias += learningRate * error;

        // Store calculated output
        outputValues[i] = output;

        // Print training results
        Serial.print("Input: (");
        Serial.print(x1); Serial.print(", ");
        Serial.print(x2); Serial.print(") → Output: ");
        Serial.print(output); Serial.print(" | Expected: ");
        Serial.println(target);

        // Delay for visualization
        delay(500);
    }
}

// Function to check perceptron output for user input
// Checks the perceptron output based on the current state of the input pushbuttons and lights up the output LED accordingly.
void checkOutput() {
    int x1 = (digitalRead(input1Pin) == HIGH) ? 1 : 0;
    int x2 = (digitalRead(input2Pin) == HIGH) ? 1 : 0;

    int index = (x1 * 2) + x2; // Convert (x1, x2) to array index (0-3)
    int output = outputValues[index]; // Fetch stored output

    digitalWrite(outputLed, output); // Light up LED if output is 1

    //Serial.print("Input: (");
    //Serial.print(x1); Serial.print(", ");
    //Serial.print(x2); Serial.print(") → Output: ");
    //Serial.println(output);
}

// Function to display training results on LEDs
// Smoothly transitions the LED brightness based on the updated weights and bias
void showTrainingResults() {
    float stepSize = 0.1; // Transition step size

    // LED Brightness based on updated weights and bias as well as previous values
    for (float t = 0; t <= 1.0; t += stepSize) {
        float smoothW1 = prevW1 + t * (w1 - prevW1);
        float smoothW2 = prevW2 + t * (w2 - prevW2);
        float smoothBias = prevBias + t * (bias - prevBias);

        // If the value is negative, set LED brightness to 0
        int w1LED = (smoothW1 > 0) ? constrain(map(smoothW1 * 100, 0, 20, 10, 255), 0, 255) : 0;
        int w2LED = (smoothW2 > 0) ? constrain(map(smoothW2 * 100, 0, 20, 10, 255), 0, 255) : 0;
        int biasLED = (smoothBias > 0) ? constrain(map(smoothBias * 10, 0, 5, 10, 255), 0, 255) : 0;

        // Update LED brightness
        analogWrite(biasLed, biasLED);
        analogWrite(weight1Led, w1LED);
        analogWrite(weight2Led, w2LED);

        delay(50); // Smooth transition delay
    }

    // Store previous values
    prevW1 = w1;
    prevW2 = w2;
    prevBias = bias;

    // Print updated weights and bias
    Serial.print("Updated Weights → w1: ");
    Serial.print(w1);
    Serial.print(", w2: ");
    Serial.print(w2);
    Serial.print(", bias: ");
    Serial.println(bias);
}

