#include <iostream> 
#include <vector> 
#include <string> 
#include <algorithm> 
#include <numeric> 
 
using namespace std; 
 
struct PhoneRecord { 
    string phone; 
    string numericPhone; 
    string value; 
 
     
    PhoneRecord() = default; 
 
     
    PhoneRecord(string phone, string value)  
        : phone(move(phone)), value(move(value)) { 
        for (char ch : this->phone) { 
            if (isdigit(ch)) { 
                numericPhone.push_back(ch); 
            } 
        } 
    } 
}; 
 
void radixSort(vector<PhoneRecord>& records) { 
    const int maxDigits = 15;  
    vector<int> indices(records.size()); 
    iota(indices.begin(), indices.end(), 0);  
 
    for (int exp = 0; exp < maxDigits; ++exp) { 
        vector<vector<int>> buckets(10); 
        for (int idx : indices) { 
            int digit = 0; 
            if (records[idx].numericPhone.size() > exp) { 
                digit = records[idx].numericPhone[records[idx].numericPhone.size() - 1 - exp] - '0'; 
            } 
            buckets[digit].push_back(idx); 
        } 
         
        indices.clear(); 
        for (auto& bucket : buckets) { 
            for (int idx : bucket) { 
                indices.push_back(idx); 
            } 
        } 
    } 
 
     
    vector<PhoneRecord> sortedRecords(records.size()); 
    for (size_t i = 0; i < records.size(); ++i) { 
        sortedRecords[i] = move(records[indices[i]]); 
    } 
    records.swap(sortedRecords);  
} 
 
int main() { 

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);

    vector<PhoneRecord> records; 
    string line, phone, value; 
 
     
        while (cin >> phone >> value) { 
        records.emplace_back(phone, value); 
    }

 
     
    radixSort(records); 
 
     
    for (const PhoneRecord& record : records) { 
        cout << record.phone << "\t" << record.value << "\n"; 
    } 
 
    return 0; 
}
