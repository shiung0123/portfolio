// ��11�� 106127116 �\�h�� 10612150 �L���� ��T�G��
#include <iostream>
#include <list>
#include <iterator>
#include <ctime>
#include <cstring>
using namespace std;

#define MENU_ERROR            0
#define MENU_INCREMENTAL_LIST 1
#define MENU_INPUT_LIST       2
#define MENU_LIMIT_LIST       3
#define MENU_QUIT             4

int counter;

// �ƦC���j
void permutation(list<int> &numList, list<int> &pick, int limitLength) 
{   
    list<int>::iterator it;

    // �P�_numList�O�_�t�����ءA�S���ɫh���󦨥�
    // limiteLength��-1�ɫh���ҥθӥ\��A�_�h�L�i�H����ƦC���̤j����
    if (!numList.size() || (limitLength != -1 && pick.size() == limitLength)) {
        // basecase
        // ��ܦ��ƭp�ƾ����W�����
        cout << '[' << ++counter << ']';

        // �N���|���e��ܥX��
        for (it = pick.begin(); it != pick.end(); ++it)
            cout << " " << *it << ",";

        // �h��','�r��
        cout << "\b " << endl;

        return;
    }
     
    // �U�C�j��Ω����Ҧ��ﶵ�U�@���A�T�O�Ҧ��զX���Q���
    for (int n = numList.size(); n > 0; --n) {

        // �����ثe��������@������|(pick)��
        int num = numList.front();
        pick.push_back(num);

        //����numList���Ĥ@��
        numList.pop_front();

        // ���j�ۤv�A�åB�O�d���׳]�w��
        permutation(numList, pick, limitLength);

        // �٭�쥻���������e
        numList.push_back(num);

        // ���|pop
        pick.pop_back();
    }

    return;
}

// ���~�B�z 
void errorHandling(string message)
{
    // �p�Geof�h�j����{��
    if (cin.eof())
        exit(0);

    // ��_cin�����A 
    cin.clear();

    // �����̦h2048�Ӧr���J��\n 
    cin.ignore(2048, '\n');

    // ��ܿ��~�T��
    cout << message << endl;
}

int task1Input() 
{
    int n;
    while (1) {
        // ��ܿ�J���ܰT��
        cout << "Input: ";

        // �ШD��J�ƦrN
        cin >> n;
        
        // �ˬdcin�O�_���`�åB���T
        // �����S�O�ˬdn�O�_����A�Y����cin�N�|�^�����~   
        // ���F�קK���|���X�ӭ����20000�H�U
        if (cin && 0 < n && n < 20000)
            return n;
        errorHandling("Error: not a postive number or too large!"); 
    }
}

// ����1
int task1() 
{
    int n = task1Input();
    
    // ���ͥ�1��N���}�C�P
    list<int> numList, pick;
    for (int i = 1; i <= n; ++i)
        numList.push_back(i);

    // ��ܿ�X����
    cout << "Output" << endl;

    // �I�s�åBmaxLength��-1
    permutation(numList, pick, -1);

    //��̤ܳj�h��
    cout << "L= " << n + 1 << endl;
    
    // �^�ǵ��G��successful
    return 0;
}

void getNumber(char &c, int &numTmp) 
{
    numTmp = 0;
    // it must be a number character
    if (!isdigit(c))
        throw "invalid Character [0-9]";

    while (1) {
        if (isdigit(c)) {
            numTmp = numTmp * 10 + c - '0';        
            if (numTmp < 0)
                throw "Overflow";
        } else if (strchr("\n ,", c) != NULL)
            break;
        else
            throw "invalid Character [0-9]";
        cin.get(c);
    } 
}

void ignoreSpace(char &c)
{
    while (c == ' ')
        cin.get(c);
}

int delimiter(char &c, list<int> &retList, int &numTmp)
{
    if (strchr("\n,", c) == NULL) 
        throw "invalid Character (',', '\\n')";

    retList.push_back(numTmp);
    numTmp = 0;

    if (c == ',')  
        cin.get(c);
    else if (c == '\n')
        return 0;
    return 1;
}

void task2Input(list<int> &retList)
{
    // ���\n
    cin.ignore(1);
    
    while (1) {
        // ��ܿ�J���ܰT��
        cout << "(ex: 1,2,3,4)" << endl;
        cout << "Input: ";

        // ��J�@�ռƦrM
        int numTmp = 0;
        char c;
        try {
            cin.get(c);
            do {
                // space trim
                ignoreSpace(c);

                // number
                getNumber(c, numTmp);

                // space trim
                ignoreSpace(c);

                // '\n' or ','
            } while (delimiter(c, retList, numTmp));
            // �ˬdcin�O�_���`�åB���T�A�H��M���d��
            if (!(cin && 2 <= retList.size() && retList.size() <= 9))
                throw "!(2 <= m <= 9) or cin error";
            else
                break;
        } catch (const char *err){
             
            // cout << err << c << endl;
            if (c != '\n')
                cin.ignore(2048, '\n'); 
            // �p�Geof�h�j����{��
            if (cin.eof())
                exit(0);

            cout << "Error: " << err << endl;
            retList.clear();
        }
    }
}

// ����2
int task2() 
{
    clock_t timer1, timer2;
         
    // �s���JM�ӼƦr
    // �̧Ƕ�J��J���Ʀr
    list<int> numList;
    task2Input(numList);

    // ���ͪŪ�pickList
    list<int> pick;
    
    // ��ܿ�X����
    cout << "Output" << endl;

    // �p�ɾ�1�p��
    timer1 = clock();
    
    // �I�s�åBmaxLength��-1
    permutation(numList, pick, -1);

    // �p�ɾ�2�p��
    timer2 = clock();

    // ��ܮɶ��t
    cout << "T= " << (float) (timer2 - timer1) * 1000 / CLOCKS_PER_SEC << "ms" << endl;      

    // �^�ǵ��G��successful
    return 0;
}

int task3Input() 
{
    int m;
    while (1) {
        // ��ܿ�J���ܰT��
        cout << "Input: ";

        // �ШD��J�ƦrM
        cin >> m;
        
        // �ˬdcin�O�_���`�åB���T�A�H��M���d��
        if (cin && (0 < m && m <= 9))
            return m;
        errorHandling("Error: out of range!"); 
    }
}

// ����3
int task3() 
{
    clock_t timer1, timer2;
    int m = task3Input();
    
    // ���ͤj�p��9��numlist�P���ͪŪ�pickList
    list<int> numList, pick;
    for (int i = 1; i <= 9; ++i)
        numList.push_back(i);

    // ��ܿ�X����
    cout << "Output" << endl;

    // �p�ɾ�1�p��
    timer1 = clock();
    
    // �I�s�åBmaxLength��M
    permutation(numList, pick, m);

    // �p�ɾ�2�p��
    timer2 = clock();

    // ��ܮɶ��t
    cout << "T= " << (float) (timer2 - timer1) * 1000 / CLOCKS_PER_SEC << "ms" << endl;      

    // �^�ǵ��G��successful
    return 0;
}

int main(int argc, char *argv[])
{
    int mode;                           // ���ﶵ
    int result;                         // ���O�^���ˬd
    while (1) {
        //��l�Ʀ��ƭp�ƾ�
        counter = 0;

        // ��X���
        cout << "           MENU           " << endl;
        cout << "* 1. N numbers from 1..N *" << endl;
        cout << "* 2. M numbers from input*" << endl;
        cout << "* 3. M numbers from 1..9 *" << endl;
        cout << "* 4. Quit                *" << endl;
        cout << "choice: ";

        // ��J���
        cin >> mode;
         
        // �P�_��ܪ����e
        switch (mode) {
            case MENU_QUIT:
                return 0;               // �h�X
            case MENU_INCREMENTAL_LIST:
                result = task1();       // ����1
                break;
            case MENU_INPUT_LIST:
                result = task2();       // ����2
                break;
            case MENU_LIMIT_LIST:
                result = task3();       // ����3
                break;
            default:
                errorHandling("Error: Command not found!"); 
                continue;
        }

        // �ˬd�^�ǭȬO�_��successful
        if (result)
            return 1;                   
    };
    return 0;
}


