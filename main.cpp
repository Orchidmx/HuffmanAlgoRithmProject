#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <memory>

// Node for Huffman Tree
struct Node {
    char ch; // Character
    int freq; // Frequency
    std::shared_ptr<Node> left, right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Compare function for priority queue
struct Compare {
    bool operator()(std::shared_ptr<Node> const& a, std::shared_ptr<Node> const& b) {
        return a->freq > b->freq;
    }
};

// Generate the Hoffman codes (later will be printed)
void generateCodes(std::shared_ptr<Node> root, const std::string& code, std::unordered_map<char, std::string>& huffmanCodes) {
    if (!root) return;

    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = code;
    }

    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

// ENCODES
std::string encode(const std::string& text, std::unordered_map<char, std::string>& huffmanCodes) {
    std::string encodedText;
    for (char ch : text) {
        encodedText += huffmanCodes[ch];
    }
    return encodedText;
}

// DECODES
std::string decode(const std::string& encodedText, std::shared_ptr<Node> root) {
    std::string decodedText;
    std::shared_ptr<Node> currentNode = root;

    for (char bit : encodedText) {
        currentNode = (bit == '0') ? currentNode->left : currentNode->right;

        if (!currentNode->left && !currentNode->right) {
            decodedText += currentNode->ch;
            currentNode = root;
        }
    }

    return decodedText;
}



int main() {
    // Reads INPUT froim the text file
    std::ifstream inputFile("original_text.txt");
    std::string text((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Counts the  character frequencies
    std::unordered_map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }

    // Create a priority queue
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, Compare> pq;

    for (const auto& pair : freqMap) {
        pq.push(std::make_shared<Node>(pair.first, pair.second));
    }

    // Builds the  Huffman Tree
    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();

        auto newNode = std::make_shared<Node>('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    auto root = pq.top();

    // Make Huffman codes visible (our Guide/legend)
    std::unordered_map<char, std::string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    // Print Huffman codes to the console terminal
    std::cout << "Huffman Codes:" << std::endl;
    for (const auto& pair : huffmanCodes) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    //  Huffman codes to a file (just as a failsafe)
    std::ofstream codesFile("huffman_codes.txt");
    for (const auto& pair : huffmanCodes) {
        codesFile << pair.first << ": " << pair.second << "\n";
    }
    codesFile.close();

    // Encode the text
    std::string encodedText = encode(text, huffmanCodes);

    // Save encoded text to file
    std::ofstream encodedFile("encoded_output.txt");
    encodedFile << encodedText;
    encodedFile.close();

    // Decode the text
    std::string decodedText = decode(encodedText, root);

    // Save decoded text to file
    std::ofstream decodedFile("decoded_output.txt");
    decodedFile << decodedText;
    decodedFile.close();


    std::cout << "Encoded and Decoded completed" << std::endl;




    return 0;
}
