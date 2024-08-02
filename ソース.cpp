#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>

using namespace std;

// 商品情報を登録するクラス
class Product {
public:
    int honnsuu;          // 1箱に入っている本数
    double naiyouryou;  // 1本あたりの内容量(mL)
    double nedann;      // 1箱あたりの値段(円)

    // 100mLあたりの値段を計算する関数
    double nedann100ml() const {
        return (nedann / (honnsuu * naiyouryou)) * 100;
    }
};

// エラー処理をする関数(整数)
void errorSyori(int& value, const string& message) {
    while (true) {
        cout << message;
        cin >> value;
        if (cin.fail() || value <= 0) { 
            //エラーの記憶をcin変数から消去する処理
            cin.clear();
            //バッファに入っているものを消去する処理
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "※ 正の整数を入力してください。" << endl;
        }
        else {
            break;
        }
    }
}
// エラー処理をする関数(浮動小数点数用)
void errorSyori(double& value, const string& message) {
    while (true) {
        cout << message;
        cin >> value;
        if (cin.fail() || value < 0.0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "※ 正の数を入力してください。" << endl;
        }
        else {
            break;
        }
    }
}

// 入力が正しいかを確認する関数
bool Nyuryokukakunin() {
    string kakunin; // 確認の入力を格納する変数
    while (true) {
        cin >> kakunin; // 使用者が「yes」か「no」を入力
        if (kakunin == "yes") {
            return true; // 「yes」が入力された場合、true
        }
        else if (kakunin == "no") {
            return false; // 「no」が入力された場合、false
        }
        else {
            cout << "※ 「yes」または「no」を入力してください。: ";
        }
    }
}

// 商品の情報を入力する関数
Product inputProduct(int Number) {
    Product product;
    bool kakutei = false;

    while (!kakutei) {
        errorSyori(product.honnsuu, "商品" + to_string(Number) + ": 1箱に入っている本数(本)を入力してください。: ");
        errorSyori(product.naiyouryou, "商品" + to_string(Number) + ": 1本あたりの内容量(mL)を入力してください。: ");
        errorSyori(product.nedann, "商品" + to_string(Number) + ": 1箱あたりの値段(円)を入力してください。: ");

        cout << "商品" << Number << "は " << product.naiyouryou << "mL、" << product.honnsuu << "本入り、"
            << product.nedann << "円で間違いありませんか？" << endl;
        cout << "間違いがなければ「yes」を、修正したい場合は「no」を入力してください。: ";

        kakutei = Nyuryokukakunin();
    }

    return product;
}

// 結果をファイルに保存する関数
void savefile(const vector<Product>& products, size_t index) {
    ofstream outFile("result.txt");
    for (size_t i = 0; i < products.size(); ++i) {
        // それぞれの商品の100mLあたりの値段をファイルに書き込む
        outFile << "商品" << i + 1 << "の100mlあたりの値段: " << products[i].nedann100ml() << "円\n";
    }
    // 最も安い商品の情報をファイルに書き込む
    outFile << "最も安い商品は商品" << index + 1 << "で、100mLあたり"
        << products[index].nedann100ml() << "円です。\n";

    outFile.close();
}

// 結果をファイルから読み込んで表示する関数
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
    vector<Product> products; //商品の情報を格納するベクター
    string input; // 入力を格納する変数
    int syouhinsuu; // 比較する商品の数

    while (true) {
        cout << "比較する商品の数を入力してください。: ";
        getline(cin, input);

        try {
            syouhinsuu = stoi(input); // 入力されたものを整数に変換
            if (syouhinsuu <= 0) {
                throw invalid_argument("Invalid input"); // 正の整数以外の場合は例外
            }
            break; // 正の整数が入力された場合は、ループを終了
        }
        catch (exception& e) {
            cout << "※ 正の整数を入力してください。" << endl;
        }
    }

    for (int i = 0; i < syouhinsuu; ++i) {
        //商品の情報を入力させる
        products.push_back(inputProduct(i + 1));
    }

    // 最も安い商品を見つける
    auto saiyasu = products.begin();  // 最初の商品を最も安い商品として仮に定義
    for (auto yasu = products.begin() + 1; yasu != products.end(); ++yasu) {
        if (yasu->nedann100ml() < saiyasu->nedann100ml()) {
            saiyasu = yasu;  // より安い商品があればそのイテレータに更新する
        }
    }

    size_t index = distance(products.begin(), saiyasu);  
     // 結果をファイルに保存する
    savefile(products, index);
    // ファイルから読み込んで表示する
    loadfile();
    return 0;
}