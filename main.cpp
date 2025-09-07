// Project UID db1f506d06d84ab787baf250c265e24e
//  main.cpp
//  p5-ml
//
//  Created by Jade Vanhaitsma on 4/10/23.
//

#include <iostream>
#include <map>
#include <set>
#include <cmath>
#include <sstream>
#include "csvstream.h"

#include "cassert"


using namespace std;


//train
// --train on ___ examples
//cout << "train on " << << "examples" << endl;

//debug
// --training data
// --label and content
// --vocab
// -- classes size
// -- classes parameters

//test
//--test data
//--correct and content
//--num correct over total

set<string> unique_words(const string &str) {
  istringstream source(str);
  set<string> words;
  string word;
  while (source >> word) {
    words.insert(word);
  }
  return words;
}



class Classifier {
private:
    bool debuggy;
    int numPosts;
    map<string, string> mappy;
    map<string, int> numOfWords;
    map<string, int> numOfTags;
    map<pair<string,string>, int> numWithTagandWord;
    set<string> dictionary;
    int vocab_size;
    map <string,string> testData;



    
    
public:
    
    Classifier (bool debug)
    : debuggy(debug), numPosts(0) {}
    
    // for each word num of words in entire training set
    void wordCounts(const string &content) {
        set<string> allWords;
        allWords = unique_words(content);
        for (const auto &word : allWords) {
            numOfWords[word] += 1;
        }
    }
    //for each label and word num of posts w label that contain word
    void countAll(const string &content, const string &tag) {
        set<string> allWords;
        allWords = unique_words(content);
        for (const auto &word : allWords) {
            numWithTagandWord[{tag, word}] += 1;
        }
    }
    
    //adding all unique words to the dictonary
    void all_unique_words(const string &str) {
      istringstream source(str);
      string word;
      while (source >> word) {
        dictionary.insert(word);
      }
    }
    
    void train(string trainingFile) {
        csvstream csvin(trainingFile);
        if (debuggy) {
            cout << "training data:" << endl;
        }
        while (csvin >> mappy) {
            const string &tag = mappy["tag"];
            numOfTags[tag] += 1; // num posts w label
            const string &content = mappy["content"]; //we want these words in the set
            all_unique_words(content);
            wordCounts(content);
            countAll(content, tag);
            numPosts++;
            if (debuggy) {
                cout << "  label = " << tag << ", content = " << content << endl;;
            }
        }
        vocab_size = (int)dictionary.size();
        cout << "trained on " << numPosts << " examples" << endl;
    }
    
    
    void debug() {
        cout << "vocabulary size = " << vocab_size << endl << endl;
        cout << "classes:" << endl;
        //LOG-PRIOR
        for (const auto &trainingData : numOfTags) {
            cout << "  " << trainingData.first << ", " << trainingData.second
            << " examples, log-prior = " << log_prior(trainingData.first) << endl;
        }
        
        //LOG-LIKELIHOOD
        cout << "classifier parameters:" << endl;

        for (const auto &trainingData : numWithTagandWord) {
            cout << "  " << (trainingData.first).first
            << ":" << (trainingData.first).second <<", count = "
            << trainingData.second
            << ", log-likelihood = "
            << log_likely((trainingData.first).first,
                          (trainingData.first).second)
            << endl;
        }
        
    }

    double log_prior(string tag) {
        double numTrainingWithC = numOfTags[tag];
        double logy = log(numTrainingWithC/numPosts);

        return logy;
    }
    
    double log_likely(string tag, string word) {
        double num = numWithTagandWord[{tag, word}];
        int dem = numOfTags[tag];
        double logy = log(num/dem);
        return logy;
    }
    
    double log_likely_other(string word) {
        double logy = 0;
        if (dictionary.find(word) == dictionary.end()) {
            logy = log((double)1/numPosts);
            //find how to get training posts with word in it
        } else {
            logy = log((double)numOfWords[word]/numPosts);
        }
        return logy;
    }
//numWithTagandWord[{trainingData.first, word}] != 0
    pair<string, double> predict(set<string> testData) {
        double prob;
        string word;
        string prediction;
        double maxProb = -INFINITY;

        for (const auto &trainingData : numOfTags) {
            prob = log_prior(trainingData.first);
            for (const auto &word : testData) {
                if (log_likely(trainingData.first, word) != log(0)) {
                    prob += log_likely(trainingData.first, word);
                } else {
                   prob += log_likely_other(word);
                }
            }

            if (prob > maxProb) {
                maxProb = prob;
                prediction = trainingData.first;
            }
        }
        
        return {prediction, maxProb};
    }
    
    void test(string testFile) {
        int numCorrect = 0;
        int totalPosts = 0;
        set<string> test_dict;
        cout << endl << "test data:" << endl;

        csvstream csvin(testFile);
        while (csvin >> testData) {
            const string &content = testData["content"];
            test_dict = unique_words(content);
            pair<string, double> likely_tag = predict(test_dict);

            cout << "  correct = " << testData["tag"]
            << ", predicted = " << likely_tag.first
            << ", log-probability score = " << likely_tag.second
            << endl
            << "  content = " << content
            << endl << endl;
            if (testData["tag"] == likely_tag.first) {
                numCorrect++;
            }
            totalPosts++;
        }

        cout << "performance: "
        <<  numCorrect << " / " <<  totalPosts
        << " posts predicted correctly"
        << endl;
        
        return;
    }
    
    
    
    
    //log likely = ln(training posts with word/training posts with label),
    //if -inf (not posts with label (bottom is 0))
    // ----use ln(posts w ith word/total training)
    // ----word is not in training = ln(1/total posts)
    //log prob = ln(prob of label) + ln (prob of word given the label)...
    //--for all unique words
    //log prior = ln (training with label/training posts)
    

    void test_cases(){
        cout << "num of words in post" << endl << endl;
        for (auto &p : numOfWords) {
            auto &word = p.first; //key
            auto &number = p.second; //value
            cout << word << " " << number << endl;
        }
        
        cout << "num of tags in post" << endl << endl;
        for (auto &p : numOfTags) {
            auto &word = p.first; //key
            auto &number = p.second; //value
            cout << word << " " << number << endl;
        }
        
        
        cout << "num of tags with label in post" << endl << endl;
        for (auto &p : numWithTagandWord) {
            auto &label = (p.first).first; //key
            auto &word = (p.first).second; //key

            auto &number = p.second; //value
            cout << label << " " << word << " " << number << endl ;
        }
    }
    
    
};

int main(int argc, char * argv[]) {
    cout.precision(3);
    bool debuggy = false;
    string debug;

    if (argc > 4 || argc < 3) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }

     if (argc == 4) {
        debug = argv[3];
        debuggy = true;
    }

    if (argc == 4 && debug != "--debug") {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }
    
    string trainFile = argv[1];
    string testFile = argv[2] ;
        
    try {
        csvstream csvin(trainFile);
    }
    catch (const csvstream_exception &e) {
        cout << "Error opening file: " << trainFile << endl;;
        return 1;
    }
    
    try {
        csvstream csvin(testFile);
    }
    catch (const csvstream_exception &e) {
        cout << "Error opening file: " << testFile << endl;
        return 1;
    }

    Classifier piazzawizz(debuggy);
    piazzawizz.train(trainFile);
    if (debuggy){
        piazzawizz.debug();
    }
    piazzawizz.test(testFile);
    
    return 0;
}
