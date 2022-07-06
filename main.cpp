#include<bits/stdc++.h>
using namespace std;

class BitwiseWrite{

	private:

	  char buf;
	  int Tbits;
	  ostream& out;

	public:

        BitwiseWrite(ostream& os) : out(os),buf(0),Tbits(0){   
            
            //clear buffer and bit counter
            
        }

        void flush();   	 // write buffer into output file and clear it 
        void writeBit(int i);
};



void BitwiseWrite::flush(){
    out.put(buf);
    out.flush();
    buf = Tbits = 0;
}

// Write the least significant bit of the argument to the bit buffer

void BitwiseWrite::writeBit(int i){ 
	if( i == -1){     //for last bit if buffer is not full still flush buffer
		flush();
	}
    if(Tbits == 8){    //if buffer full(8 bit) clear buffer
        flush();
    }
    
    //if the int is 1, we must put the bit 1 into the buffer
    unsigned char mask = 1;
    if(i == 1) {
        mask = mask << (7 - Tbits);
        buf = buf | mask;
    } 
    //if the int is 0, we must put the bit 0 into the buffer
    if(i == 0) {
        mask = mask << (7 - Tbits);
        mask = ~mask;
        buf = buf & mask;
    } 
    Tbits++;             //increment Tbits++
}

class BitwiseRead {

    private:

        char buf;
        int nbits;        //how many bits have been read from the buffer
        istream& in;

    public:

        /* Initialize a BitwiseRead that will use the given istream for input
        * */
        BitwiseRead(istream & is);

        /* Fill the buffer from the input */
        void fill();
        
        /* Read the next bit from the bit buffer. Fill the buffer form the input
        * stream first if needed.
        * Return 1 if the bit read is 1, 0 if bit read is 0
        * */
        int readBit();
};

BitwiseRead::BitwiseRead(istream & is) : in(is){
    buf = 0;
    nbits = 8;
}

void BitwiseRead::fill(){
	buf = in.get();
	nbits = 0;
}

int BitwiseRead::readBit(){
    if(nbits == 8) {
        fill();
 }
  
    //uses a mask to extract the nbits'th bit
    unsigned char mask = 1;
    mask = mask << (7-nbits); 
    mask = mask & buf;
    nbits++;
    if(mask == 0) {
        return 0;
    }
    else {
        return 1;
    } 
}



struct HeapNode{ //define a node data structure for binary tree 

    int freq;
    int data;
    HeapNode* left;
    HeapNode* right;

    HeapNode(int freq, int data){
        this->left = NULL;
        this->right = NULL;
        this->freq = freq;
        this->data = data;
    }
};

struct MinHeap{ //define minheap data structure

    vector<HeapNode*> arr;

    MinHeap(vector<HeapNode*> arr){
        this->arr = arr;
    }

    void heapify(int i){
        int l = 2*i + 1;
        int r = 2*i + 2;
        int smallest = i;
        if(l < arr.size() && arr[l]->freq < arr[smallest]->freq)
        smallest = l;
        if(r < arr.size() && arr[r]->freq < arr[smallest]->freq)
        smallest = r;
        if(smallest != i)
        {
            HeapNode* temp = arr[i];
            arr[i] = arr[smallest];
            arr[smallest] = temp;
            heapify(smallest);
        }
    }

    void build_heap(){
        for(int i = arr.size()/2 - 1; i>=0; i--)
        heapify(i);
    }

    HeapNode* pop(){
        if(arr.size() == 0)
        {
            return NULL;
        }
        HeapNode* min = arr[0];
        arr[0] = arr[arr.size() - 1];
        arr.pop_back();
        heapify(0);
        return min;
    }

    void push(HeapNode* key){
        arr.push_back(key);
        int i = arr.size() - 1;
        while(i > 0 && arr[i]->freq < arr[(i-1)/2]->freq)
        {
            HeapNode* temp = arr[i];
            arr[i] = arr[(i - 1)/2];
            arr[(i - 1)/2] = temp;
            i = (i - 1)/2;
        }
    }

    int heapSize(){
        return arr.size();
    }
};

unordered_map<string,int> mp; // map to store binarycode for characters

struct FileCompression{  // file cmopression part

    string filename;
    string fname = "";

    FileCompression(string filename){
        this->filename=filename;
        int i = 0;
        while(filename[i]!='.'){
            this->fname += filename[i];
            i++;
        }
    }

    void frequency_map(string text, vector<int>& freq){
        for(auto c : text){
            freq[int(c)]++;
        }
    }

    void binaryTree(MinHeap* minheap){
        while(minheap->heapSize() != 1){
            HeapNode* Left = minheap->pop();
            HeapNode* Right = minheap->pop();
            HeapNode* Top = new HeapNode(Left->freq + Right->freq, 400);
            Top->left = Left;
            Top->right = Right;
            minheap->push(Top);
        }
    }

    void binaryCode(HeapNode* root, string bit, vector<string>& codes){
        if(root == NULL){
            return;
        }
        if(root->data != 400){
            codes[root->data] = bit;
            mp[bit] = root->data;
        }
        binaryCode(root->left, bit+'0', codes);
        binaryCode(root->right, bit+'1', codes);
    }

    string encodedString(string txt, vector<string>& codes){
        string encode = "";
        for(char c : txt){ 
            encode += codes[int(c)];
        }
        return encode;
    }

    int binaryToInt(string s){
        int num = 0;
        for(int i = s.size()-1; i >= 0; i--){
            num += pow(2, s.size() - i - 1)*(s[i] - '0');
        }
        return num;
    }

    void bytesArray(string text, vector<int>& bytes){
        for(auto s : text){
            bytes.push_back(s - '0');
        }
    }
    void compress(){
        cout<<"\nProcess Starts for Compressing!!" <<endl;
        string text;
        ifstream in(filename);
        while(in.eof() == 0){
            string s;
            getline(in, s);
            text += s;
        }
        in.close();
        vector<int> freq(256, 0);
        frequency_map(text, freq);
        vector<HeapNode*> heapnode;
        for(int i = 0; i < freq.size(); i++){
            if(freq[i]>0){
                HeapNode* h = new HeapNode(freq[i], i);
                heapnode.push_back(h);
            }
        }
        MinHeap* minheap = new MinHeap(heapnode);
        minheap->build_heap();
        binaryTree(minheap);
        vector<string> codes(256);
        binaryCode(minheap->pop(), "", codes);
        string encodetxt = encodedString(text, codes);
        vector<int> bytes;
        bytesArray(encodetxt,bytes);
        ofstream out;
        out.open(fname+".bin",ios::binary);
        int temp1;
        BitwiseWrite s(out);
        cout<<"\nProcessing.... Plz wait !!!\n";
        for(int d = 0;d < bytes.size();d++) {
            temp1 = bytes[d];
            s.writeBit(temp1);           
        }
        out.close();
        cout<<"\nSuccessfully Compressed!!";
    }
};


struct FileDecompression{ //file decompression part

    string filename;
    string fname="";

    FileDecompression(string filename){
        this->filename=filename;
        int i = 0;
        while(filename[i]!='.'){
            this->fname += filename[i];
            i++;
        }
    }

    void decompress(){
        cout<<"\nProcess Starts for Decompressing!!" <<endl;
        ifstream in;
        in.open(filename, ios::binary);
        BitwiseRead os(in);
        vector<int> bytes;
        while(in.eof() == 0){
          bytes.push_back(os.readBit());
        }
        cout<<"\nProcessing.... Plz wait !!!\n";
        string text="";
        string sub="";
        for(int i=0;i<bytes.size();i++){
            sub += (bytes[i]+'0');
            if(mp.find(sub)!=mp.end()){
                text += mp[sub];
                sub="";
            }
        }
        ofstream out(fname+"-Decompressed.txt");
        out<<text;
        out.close();
        in.close();
        cout<<"\nSuccessfully Decompressed!!\n";
    }
};

int main(){
    string name;
    cout<<"\nEnter the name of the file that you want to compresse: ";
    cin>>name;
    FileCompression* f = new FileCompression(name);
    f->compress();
    cout<<"\n\nEnter the name of the file that you want to decompressed:  ";
    cin>>name;
    FileDecompression* df = new FileDecompression(name);
    df->decompress();
    return 0;
}