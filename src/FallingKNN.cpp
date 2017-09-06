#include "FallingKNN.h"

//calculate the distance between test data and lib[i]
double FallingKNN:: get_distance(int16_t d1[],int16_t d2[]) {
    double sum = 0;
    for(int i = 0;i < COL_NUM;i++) {
        sum += pow(abs(d1[i]) - abs(d2[i]),2);
    }
    return sqrt(sum);
}


//calculate all the distance between test data and each training data
void FallingKNN::get_all_distance(int16_t ax, int16_t ay, int16_t az) {
    /* inuput the test data */
    testData[0] = ax;
    testData[1] = ay;
    testData[2] = az;
    int16_t temp_accel[3];
    double tempDis;
    char tempLabel;
    // disWithLabel: pair<label,distance>
    // dis_lib: vector(pair<label1,distance1>,pair<label2,distance2>...)
    DisPair disWithLabel;
    for(int i = 0; i < ROW_NUM; i++) {
      temp_accel[0] = pgm_read_word_near(lib[i]);
      temp_accel[1] = pgm_read_word_near(lib[i] + 1);
      temp_accel[2] = pgm_read_word_near(lib[i] + 2);
      
      tempDis = get_distance(temp_accel,testData);
      tempLabel = (char) pgm_read_byte_near(lib[i] + COL_NUM);
      
      if(i < k) {
      disWithLabel.first = tempLabel;
      disWithLabel.second = tempDis;
      dis_lib.push_back(disWithLabel);
      } else {
        sort(dis_lib.begin(),dis_lib.end(),CmpByDis());
        if(dis_lib[k-1].second > tempDis) {
          dis_lib[k-1].first = tempLabel;
          dis_lib[k-1].second = tempDis;
        }
      }
    }
}

// To sort the distance from min to max
void FallingKNN::dis_sort() {
    FreqPair label_pair;
    char currentLabel;
    for (int i = 0; i < k; i++) {
        //calculate the count of each label
        currentLabel = dis_lib[i].first;
        if(i == 0) {
            label_pair.first = currentLabel;
            label_pair.second = 1;
            label_freq.push_back(label_pair);
        } else {
            for (int j = 0; j < label_freq.size(); j++) {
                if (label_freq[j].first == currentLabel) {
                    label_freq[j].second++;
                    break;
                } else if (j == label_freq.size() - 1) {
                    label_pair.first = currentLabel;
                    label_pair.second = 1;
                    label_freq.push_back(label_pair);
                    break;
                }
            }
        }
    }
}

// Check which label the test data belongs to to classify the test data
char FallingKNN::get_max_freq_label() {
    // Sort label_freq by frequency
    sort(label_freq.begin(), label_freq.end(), CmpByFreq());
    char label;
    int8_t max_freq = 0;
    //find the most frequent label
    if(label_freq[0].second != label_freq[1].second) {
        label = label_freq[0].first;
        max_freq = label_freq[0].second;
        isUnique = true;
    } else {
        isUnique = false;
        k += 2;
    }
    return label;
}

char FallingKNN::get_label(int16_t ax,int16_t ay,int16_t az) {
    char label;
    dis_lib.clear();
    label_freq.clear();
    do {
        get_all_distance(ax,ay,az);
        dis_sort();
        label = get_max_freq_label();
    } while(!isUnique);
    return label;
}

