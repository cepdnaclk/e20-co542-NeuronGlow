// Implementation of a Single Layer Perceptron
// Uses two potentiometers as inputs and a pushbutton to trigger training
// Outputs the result on an LED

// VERSION 01

// Pin definitions
const int pot1Pin = A0;         // Potentiometer 1
const int pot2Pin = A1;         // Potentiometer 2
const int epochButtonPin = 4;   // Pushbutton to trigger epochs
const int outputLed = 5;        // LED for Output
const int weight1Led = 7;       // LED for weight1
const int weight2Led = 8;       // LED for weight2
const int biasLed = 6;          // LED for bias

// Perceptron variables

// Starting values for weights and bias (can be changed to any value)
float w1 = 3.0, w2 = 3.0, bias = 2.0; 

// Previous values for smooth transition of brightnesses
// Advisable to use the same values as starting weights and bias
float prevW1 = w1, prevW2 = w2, prevBias = bias;

// Learning rate and epoch count
float learningRate = 1; 
int epochcount = 0;

// SampleCount is the number of samples to train on each epoch
const int sampleCount = 10;

// Arrays to store trained values
// trainedPot1 and trainedPot2 store the potentiometer values
float trainedPot1[sampleCount];
float trainedPot2[sampleCount];

// trainedLabels store the corresponding output labels
int trainedLabels[sampleCount];

// Count of trained samples
int trainedCount = 0;

// Setup Function
// Initializes Serial communication and pin modes for inputs and outputs
void setup() {
    Serial.begin(9600);
    pinMode(epochButtonPin, INPUT);
    pinMode(outputLed, OUTPUT);
    pinMode(weight1Led, OUTPUT);
    pinMode(weight2Led, OUTPUT);
    pinMode(biasLed, OUTPUT);
    Serial.println("Perceptron Training Ready. Press button to train on 5 random samples.");
}

// Main Loop Function
// Checks if the epoch button is pressed to start training
// Checks the perceptron output based on the potentiometer inputs
void loop() {
    if (digitalRead(epochButtonPin) == HIGH) { // Epoch button pressed
        delay(200); // Debounce delay
        trainOnRandomValues();  // Train the perceptron on random values
        updateWeightsAndBias(); // Update weights and bias based on training
        showTrainingResults(); // Show the training results on LEDs
    } else {
        checkOutput(); // Check perceptron output for live potentiometer inputs
    }
    delay(100);
}

// Function to train the perceptron on random values
// Generates random training samples for the potentiometers.
// Assigns a label of 1 to each sample. 
// It also prints the training samples to the serial monitor.
void trainOnRandomValues() {
    trainedCount = 0;
    for (int i = 0; i < sampleCount; i++) {
        trainedPot1[i] = random(400, 700) / 1023.0; // Random values in 0.4-0.7 range
        trainedPot2[i] = random(400, 700) / 1023.0; // Same for Pot2
        trainedLabels[i] = 1;                       // Label is 1 for all samples generated
        trainedCount++;                             // Increment the trained count\
        // Print the training samples to the Serial Monitor
        Serial.print("Trained on Pot1: "); Serial.print(trainedPot1[i]);
        Serial.print(" Pot2: "); Serial.print(trainedPot2[i]);
        Serial.println(" as Output: 1");
    }
}

// Function to update weights and bias based on training
// Uses the Perceptron Learning Rule to update weights and bias
// The learning rate is used to control the rate of learning
void updateWeightsAndBias() {
    // Simple training logic to update weights and bias
    for (int i = 0; i < trainedCount; i++) {
        float pot1 = trainedPot1[i];
        float pot2 = trainedPot2[i];
        int label = trainedLabels[i];
        
        // Perceptron learning rule
        float output = (pot1 * w1 + pot2 * w2 + bias) > 0 ? 1 : 0;
        float error = label - output;
        
        // Update weights and bias based on learning rate and error
        w1 += learningRate * error * pot1;
        w2 += learningRate * error * pot2;
        bias += learningRate * error;
    }
}

// Function to check the perceptron output for live potentiometer inputs
// Checks if the current potentiometer values match any trained value
// If a match is found, the corresponding output is displayed on the LED
void checkOutput() {
    float pot1 = analogRead(pot1Pin) / 1023.0;
    float pot2 = analogRead(pot2Pin) / 1023.0;
    int output = 0;
    
    // Check if current potentiometer values match any trained value
    for (int i = 0; i < trainedCount; i++) {
        if (abs(pot1 - trainedPot1[i]) < 0.05 && abs(pot2 - trainedPot2[i]) < 0.05) {
            output = trainedLabels[i];
            break;
        }
    }
    
    // Display the output on the LED
    digitalWrite(outputLed, output);
    //Serial.print("Live Input → Pot1: "); Serial.print(pot1);
    //Serial.print(" Pot2: "); Serial.print(pot2);
    //Serial.print(" → Output: "); Serial.println(output);
    delay(100);
}

// Function to show the training results on the LEDs
// Smoothly transitions the LED brightness based on the updated weights and bias
void showTrainingResults() {
    float stepSize = 0.5; // Transition step size

    // Smooth transition for weights and bias
    for (float t = 0; t <= 1.0; t += stepSize) {
        float smoothW1 = prevW1 + t * (w1 - prevW1);
        float smoothW2 = prevW2 + t * (w2 - prevW2);
        float smoothBias = prevBias + t * (bias - prevBias);

        // If the value is negative, set LED brightness to 0
        int w1LED = (smoothW1 > 0) ? constrain(map(smoothW1 * 100, 0, 10, 10, 255), 0, 255) : 0;
        int w2LED = (smoothW2 > 0) ? constrain(map(smoothW2 * 100, 0, 10, 10, 255), 0, 255) : 0;
        int biasLED = (smoothBias > 0) ? constrain(map(smoothBias * 10, 0, 5, 10, 255), 0, 255) : 0;

        // Set LED brightness based on weights and bias
        analogWrite(weight1Led, w1LED);
        analogWrite(weight2Led, w2LED);
        analogWrite(biasLed, biasLED);

        delay(50); // Smooth transition delay
    }

    // Print the weights and bias to the Serial Monitor
    Serial.print("Weight 1: "); Serial.println(w1);
    Serial.print("Weight 2: "); Serial.println(w2);
    Serial.print("Bias: "); Serial.println(bias);

    // Store previous values for next transition
    prevW1 = w1;
    prevW2 = w2;
    prevBias = bias;
}
