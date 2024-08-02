#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>

using namespace std;

// ���i����o�^����N���X
class Product {
public:
    int honnsuu;          // 1���ɓ����Ă���{��
    double naiyouryou;  // 1�{������̓��e��(mL)
    double nedann;      // 1��������̒l�i(�~)

    // 100mL������̒l�i���v�Z����֐�
    double nedann100ml() const {
        return (nedann / (honnsuu * naiyouryou)) * 100;
    }
};

// �G���[����������֐�(����)
void errorSyori(int& value, const string& message) {
    while (true) {
        cout << message;
        cin >> value;
        if (cin.fail() || value <= 0) { 
            //�G���[�̋L����cin�ϐ�����������鏈��
            cin.clear();
            //�o�b�t�@�ɓ����Ă�����̂��������鏈��
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�� ���̐�������͂��Ă��������B" << endl;
        }
        else {
            break;
        }
    }
}
// �G���[����������֐�(���������_���p)
void errorSyori(double& value, const string& message) {
    while (true) {
        cout << message;
        cin >> value;
        if (cin.fail() || value < 0.0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�� ���̐�����͂��Ă��������B" << endl;
        }
        else {
            break;
        }
    }
}

// ���͂������������m�F����֐�
bool Nyuryokukakunin() {
    string kakunin; // �m�F�̓��͂��i�[����ϐ�
    while (true) {
        cin >> kakunin; // �g�p�҂��uyes�v���uno�v�����
        if (kakunin == "yes") {
            return true; // �uyes�v�����͂��ꂽ�ꍇ�Atrue
        }
        else if (kakunin == "no") {
            return false; // �uno�v�����͂��ꂽ�ꍇ�Afalse
        }
        else {
            cout << "�� �uyes�v�܂��́uno�v����͂��Ă��������B: ";
        }
    }
}

// ���i�̏�����͂���֐�
Product inputProduct(int Number) {
    Product product;
    bool kakutei = false;

    while (!kakutei) {
        errorSyori(product.honnsuu, "���i" + to_string(Number) + ": 1���ɓ����Ă���{��(�{)����͂��Ă��������B: ");
        errorSyori(product.naiyouryou, "���i" + to_string(Number) + ": 1�{������̓��e��(mL)����͂��Ă��������B: ");
        errorSyori(product.nedann, "���i" + to_string(Number) + ": 1��������̒l�i(�~)����͂��Ă��������B: ");

        cout << "���i" << Number << "�� " << product.naiyouryou << "mL�A" << product.honnsuu << "�{����A"
            << product.nedann << "�~�ŊԈႢ����܂��񂩁H" << endl;
        cout << "�ԈႢ���Ȃ���΁uyes�v���A�C���������ꍇ�́uno�v����͂��Ă��������B: ";

        kakutei = Nyuryokukakunin();
    }

    return product;
}

// ���ʂ��t�@�C���ɕۑ�����֐�
void savefile(const vector<Product>& products, size_t index) {
    ofstream outFile("result.txt");
    for (size_t i = 0; i < products.size(); ++i) {
        // ���ꂼ��̏��i��100mL������̒l�i���t�@�C���ɏ�������
        outFile << "���i" << i + 1 << "��100ml������̒l�i: " << products[i].nedann100ml() << "�~\n";
    }
    // �ł��������i�̏����t�@�C���ɏ�������
    outFile << "�ł��������i�͏��i" << index + 1 << "�ŁA100mL������"
        << products[index].nedann100ml() << "�~�ł��B\n";

    outFile.close();
}

// ���ʂ��t�@�C������ǂݍ���ŕ\������֐�
void loadfile() {
    ifstream inFile("result.txt");
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    }
}

int main() {
    vector<Product> products; //���i�̏����i�[����x�N�^�[
    string input; // ���͂��i�[����ϐ�
    int syouhinsuu; // ��r���鏤�i�̐�

    while (true) {
        cout << "��r���鏤�i�̐�����͂��Ă��������B: ";
        getline(cin, input);

        try {
            syouhinsuu = stoi(input); // ���͂��ꂽ���̂𐮐��ɕϊ�
            if (syouhinsuu <= 0) {
                throw invalid_argument("Invalid input"); // ���̐����ȊO�̏ꍇ�͗�O
            }
            break; // ���̐��������͂��ꂽ�ꍇ�́A���[�v���I��
        }
        catch (exception& e) {
            cout << "�� ���̐�������͂��Ă��������B" << endl;
        }
    }

    for (int i = 0; i < syouhinsuu; ++i) {
        //���i�̏�����͂�����
        products.push_back(inputProduct(i + 1));
    }

    // �ł��������i��������
    auto saiyasu = products.begin();  // �ŏ��̏��i���ł��������i�Ƃ��ĉ��ɒ�`
    for (auto yasu = products.begin() + 1; yasu != products.end(); ++yasu) {
        if (yasu->nedann100ml() < saiyasu->nedann100ml()) {
            saiyasu = yasu;  // ���������i������΂��̃C�e���[�^�ɍX�V����
        }
    }

    size_t index = distance(products.begin(), saiyasu);  
     // ���ʂ��t�@�C���ɕۑ�����
    savefile(products, index);
    // �t�@�C������ǂݍ���ŕ\������
    loadfile();
    return 0;
}