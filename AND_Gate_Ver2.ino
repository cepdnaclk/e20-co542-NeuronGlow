// Implementation of a Single Layer Perceptron
// AND Gate Implementation
// Uses two pushbuttons as inputs and a pushbutton to trigger training
// Outputs weights and bias on LEDs
// Main Output on LED

// VERSION 02

// Pin definitions
const int input1Pin = 2;      // Pushbutton for X1 (input 1)
const int input2Pin = 3;      // Pushbutton for X2 (input 2)
const int epochButtonPin = 4; // Pushbutton to trigger training

const int outputLed = 5;      // LED for Output
const int biasLed = 6;        // LED for Bias
const int weight1Led = 7;     // LED for Weight 1 (w1)
const int weight2Led = 8;     // LED for Weight 2 (w2);

// Perceptron variables
float w1 = 0.0, w2 = 0.0, bias = 0.0;  // Weights and bias
float learningRate = 0.1;              // Learning rate

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
        trainPerceptron();
        showTrainingResults();
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

        // Delay to visualize training
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
// Maps the weight and bias values to LED brightness levels for visualization
void showTrainingResults() {
    int w1LED = map(w1 * 100, -100, 200, 0, 255);
    int w2LED = map(w2 * 100, -100, 200, 0, 255);
    int biasLED = map(bias * 10, -5, 20, 0, 255);

    // Update LED brightness
    analogWrite(biasLed, biasLED);
    analogWrite(weight1Led, w1LED);
    analogWrite(weight2Led, w2LED);

    // Print updated weights and bias
    Serial.print("Updated Weights → w1: ");
    Serial.print(w1);
    Serial.print(", w2: ");
    Serial.print(w2);
    Serial.print(", bias: ");
    Serial.println(bias);
}
