#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

string formatNumber(double num) {
    string s = to_string(num);
    if (num == (long long)num) {
        return to_string((long long)num);
    }
    while (s.back() == '0') s.pop_back();
    if (s.back() == '.') s.pop_back();
    return s;
}


struct Node {

    double data;
    char symbol;
    string stdata;
    Node* next;

    Node(double val) : data(val), symbol('\0'), stdata(formatNumber(val)), next(nullptr) {}
    Node(char val) : data(0.0), symbol(val), stdata(""), next(nullptr) {}
    Node(string text) : data(0.0), symbol('\0'), stdata(text), next(nullptr) {}
};


template <typename T>
void pushNodeFront(T value, Node*& head, int& size) {
    Node* newNode = new Node(value);
    newNode->next = head;
    head = newNode;
    size++;
}

template <typename T>
void pushNodeBack(T value, Node* head, int& size) {
    Node* curr = head;
    Node* newNode = new Node(value);
    if (!head) {
        head = newNode;
        return;
    }
    while (curr->next != nullptr) {
        curr = curr->next;
    }
    curr->next = newNode;
    size++;
}

void popNode(Node*& head, int& size) {
    if (head == nullptr) {
        cout << "Удалять нечего";
        return;
    }
    Node* temp = head;
    head = head->next;
    delete temp;
    size--;
}

void printNode(Node* head) {
    Node* curr = head;
    while (curr) {
        cout << "Объект: " << curr << endl;
        cout << "Числовые данные: " << curr->data << endl;
        cout << "Символьные данные: " << curr->symbol << endl;
        cout << "Строковые данные: " << curr->stdata << endl;
        cout << "Следующий адрес: " << curr->next << endl;
        cout << endl;
        curr = curr->next;
    }
}

int getSize(Node* head) {
    int cnt = 0;
    Node* curr = head;
    while (curr != nullptr) {
        cnt += 1;
        curr = curr->next;
    }
    return cnt;
}

void deleteNode(Node*& head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}


double applyOperat(double a, double b, char sym) {
    switch (sym) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    }
}


bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

string removeSpaces(string text) {
    string result;
    for (size_t i = 0; i < text.size(); i++) {
        if (!isspace(text[i])) {
            result += text[i];
        }
    }
    return result;
}

double parseNumber(string& text, size_t& i) {
    string num;
    if (text[i] == '-') { num += text[i++]; }
    while ((isdigit(text[i]) || text[i] == '.') && i < text.size()) {
        num += text[i++];
    }
    i--;
    if (num.empty() || num == "-") {
        cout << "Неверное число: " + num;
    }
    return stod(num);
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

string printRPN(string& text) {
    string out_a;
    Node* OperatNode = nullptr;
    int operatSize = 0;
    bool lastWasOperator = true;
    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        if (isspace(c)) continue;
        if (isdigit(c) || (c == '-' && lastWasOperator)) {
            double num = parseNumber(text, i);
            out_a += formatNumber(num) + " "; 
            lastWasOperator = false;
        }
        else if (c == '(') {
            pushNodeFront(c, OperatNode, operatSize);
            lastWasOperator = true;
        }
        else if (c == ')') {
            while (OperatNode != nullptr && OperatNode->symbol != '(') {
                out_a += OperatNode->symbol;
                out_a += " ";
                popNode(OperatNode, operatSize);
            }
            if (OperatNode != nullptr) {
                popNode(OperatNode, operatSize);
            }
            lastWasOperator = false;
        }
        else if (isOperator(c)) {
            while (OperatNode != nullptr && OperatNode->symbol != '(' &&
                precedence(OperatNode->symbol) >= precedence(c)) {
                out_a += OperatNode->symbol;
                out_a += " ";
                popNode(OperatNode, operatSize);
            }
            pushNodeFront(c, OperatNode, operatSize);
            lastWasOperator = true;
        }
    }
    while (OperatNode != nullptr) {
        if (OperatNode->symbol != '(') {
            out_a += OperatNode->symbol;
            out_a += " ";
        }
        popNode(OperatNode, operatSize);
    }
    deleteNode(OperatNode);
    return out_a;
}

string printPN(string& text) {
    string out_a;
    Node* OperatNode = nullptr;
    Node* NumNode = nullptr;
    int operatNodeSize = 0;
    int numNodeSize = 0;
    bool lastWasOperator = true;


    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];

        if (isspace(c)) continue;

        if (isdigit(c) || (c == '-' && lastWasOperator)) {
            double num = parseNumber(text, i);
            pushNodeFront(num, NumNode, numNodeSize); 
            lastWasOperator = false;
        }
        else if (c == '(') {
            pushNodeFront(c, OperatNode, operatNodeSize);
            lastWasOperator = true;
        }
        else if (c == ')') {
            while (OperatNode != nullptr && OperatNode->symbol != '(') {
                if (numNodeSize < 2) {
                    deleteNode(OperatNode);
                    deleteNode(NumNode);
                    cout <<  "Недостаточно операндов";
                }
                char op = OperatNode->symbol;
                popNode(OperatNode, operatNodeSize);
                string op2 = NumNode->stdata; popNode(NumNode, numNodeSize);
                string op1 = NumNode->stdata; popNode(NumNode, numNodeSize);
                string result = string(1, op) + " " + op1 + " " + op2;
                pushNodeFront(result, NumNode, numNodeSize);
            }
            popNode(OperatNode, operatNodeSize); 
            lastWasOperator = false;
        }
        else if (isOperator(c)) {
            while (OperatNode != nullptr && OperatNode->symbol != '(' &&
                precedence(OperatNode->symbol) > precedence(c)) {
                char op = OperatNode->symbol;
                popNode(OperatNode, operatNodeSize);
                string op2 = NumNode->stdata; popNode(NumNode, numNodeSize);
                string op1 = NumNode->stdata; popNode(NumNode, numNodeSize);
                string result = string(1, op) + " " + op1 + " " + op2;
                pushNodeFront(result, NumNode, numNodeSize);
            }
            pushNodeFront(c, OperatNode, operatNodeSize);
            lastWasOperator = true;
        }
        else {
            deleteNode(OperatNode);
            deleteNode(NumNode);
        }
    }

    while (OperatNode != nullptr) {
        char op = OperatNode->symbol;
        popNode(OperatNode, operatNodeSize);
        string op2 = NumNode->stdata; popNode(NumNode, numNodeSize);
        string op1 = NumNode->stdata; popNode(NumNode, numNodeSize);
        string result = string(1, op) + " " + op1 + " " + op2;
        pushNodeFront(result, NumNode, numNodeSize);
    }


    out_a = NumNode->stdata;
    deleteNode(OperatNode);
    deleteNode(NumNode);
    return out_a;
}

double calculateRPN(string& text) {
    Node* stack = nullptr;
    int stackSize = 0;
    string value;
    size_t i = 0;
    while (i < text.size()) {
        char elem = text[i];

        if (elem == ' ') {
            i++;
            continue;
        }

        if (isdigit(elem) || elem == '-') {
            value.clear();
            value += elem;
            i++;
            while (i < text.size() && (isdigit(text[i]) || text[i] == '.')) {
                value += text[i];
                i++;
            }
            double res = stod(value);
            pushNodeFront(res, stack, stackSize);
            i--; 
        }

        else if (isOperator(elem)) {
            double right = stack->data; popNode(stack, stackSize);
            double left = stack->data; popNode(stack, stackSize);
            double result = applyOperat(left, right, elem);
            pushNodeFront(result, stack, stackSize);
            i++;
        }
        else {
            deleteNode(stack);
            cout << "Недопустимый символ: " + string(1, elem);
        }
        i++;
    }

    double result = stack->data;
    deleteNode(stack);
    return result;
}

double calculatePN(string& text) {
    Node* stack = nullptr;
    int stackSize = 0;
    string value;
    for (int i = text.size() - 1; i >= 0; i--) {
        char elem = text[i];
        // cout << "i = " << i << " text[i] = " << text[i] << endl;
        if (elem == ' ') {
            // cout << i << " - при пробеле" << endl;
            continue;
        }
        if (isdigit(elem) || elem == '-') {
            value.clear();
            value += elem;
            i--;
            while (i >= 0 && (isdigit(text[i]) || text[i] == '.') || text[i] == '-') { 
                value += text[i];
                i--;
            }
            reverse(value.begin(), value.end());
            double res;
            if (value.empty() || value == "-") {
                cout << "Неверное число: " << value << endl;
                deleteNode(stack);
                return 0;
            }
            try {
                res = stod(value);
            }
            catch (...) {
                cout << "Неверное число: " << value << endl;
                deleteNode(stack);
                return 0;
            }
            pushNodeFront(res, stack, stackSize);
        }
        else if (isOperator(elem)) {
            if (stackSize < 2) {
                cout << "Недостаточно операндов для оператора: " << elem << endl;
                deleteNode(stack);
                return 0;
            }
            double right = stack->data; popNode(stack, stackSize);
            double left = stack->data; popNode(stack, stackSize);
            double result = applyOperat(right, left, elem);
            pushNodeFront(result, stack, stackSize);
        }
        else {
            cout << "Недопустимый символ: " << elem << endl;
            deleteNode(stack);
            return 0;
        }
    }

    if (stackSize != 1) {
        cout << "Неверное количество операндов в PN" << endl;
        deleteNode(stack);
        return 0;
    }

    double result = stack->data;
    deleteNode(stack);
    return result;
}



bool CheckSimple(string& expression) {
    Node* stack = nullptr;
    int stackSize = 0;
    int operandCount = 0; 
    bool lastWasOperator = true; 
    bool hasNumberOrParen = false; 

    if (expression.empty()) {
        cout << "Ошибка: Пустое выражение" << endl;
        return false;
    }

    for (size_t i = 0; i < expression.length(); i++) {
        char elem = expression[i];
        if (isspace(elem)) {
            continue;
        }

        if (isdigit(elem) || (elem == '-' && lastWasOperator)) {
            string num;
            if (elem == '-') num += elem;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                num += expression[i++];
            }
            i--;
            if (num == "-" || num == "-." || num == "." || num.empty()) {
                cout << "Ошибка: Неверное число '" << num << "' в позиции " << i << endl;
                deleteNode(stack);
                return false;
            }
            int dotCount = 0;
            for (char ch : num) {
                if (ch == '.') dotCount++;
                if (dotCount > 1) {
                    cout << "Ошибка: Множественные точки в числе в позиции " << i << endl;
                    deleteNode(stack);
                    return false;
                }
            }
            operandCount++;
            lastWasOperator = false;
            hasNumberOrParen = true;
        }
        else if (elem == '(') {
            pushNodeFront(elem, stack, stackSize);
            lastWasOperator = true;
            hasNumberOrParen = true;
        }
        else if (elem == ')') {
            bool foundOpenParen = false;
            Node* current = stack;
            while (current != nullptr) {
                if (current->symbol == '(') {
                    foundOpenParen = true;
                    break;
                }
                current = current->next;
            }
            if (!foundOpenParen) {
                cout << "Ошибка: Лишняя закрывающая скобка в позиции " << i << endl;
                deleteNode(stack);
                return false;
            }
            while (stack != nullptr && stack->symbol != '(') {
                popNode(stack, stackSize);
            }
            if (stack != nullptr) {
                popNode(stack, stackSize); 
            }
            if (lastWasOperator) {
                cout << "Ошибка: Оператор перед закрывающей скобкой в позиции " << i << endl;
                deleteNode(stack);
                return false;
            }
            lastWasOperator = false;
        }
        else if (isOperator(elem)) {
            if (lastWasOperator) {
                cout << "Ошибка: Два оператора подряд в позиции " << i << endl;
                deleteNode(stack);
                return false;
            }
            if (operandCount < 1) {
                cout << "Ошибка: Недостаточно операндов перед оператором в позиции " << i << endl;
                deleteNode(stack);
                return false;
            }
            operandCount--; 
            lastWasOperator = true;
        }
        else {
            cout << "Ошибка: Недопустимый символ '" << elem << "' в позиции " << i << endl;
            deleteNode(stack);
            return false;
        }
    }

    if (stack != nullptr) {
        cout << "Ошибка: Несбалансированные скобки" << endl;
        deleteNode(stack);
        return false;
    }

    if (lastWasOperator) {
        cout << "Ошибка: Выражение заканчивается оператором" << endl;
        return false;
    }

    if (!hasNumberOrParen) {
        cout << "Ошибка: Выражение не содержит чисел или скобок" << endl;
        return false;
    }

    // Проверяем, что остался ровно один результат
    if (operandCount != 1) {
        cout << "Ошибка: Неверное количество операндов (ожидается 1, найдено " << operandCount << ")" << endl;
        return false;
    }

    deleteNode(stack);
    return true;
}

bool CheckRPN(string& expression) {
    if (expression.empty()) {
        cout << "Ошибка. Пустая обратная польская нотация" << endl;
        return false;
    }

    Node* stack = nullptr;
    int stackSize = 0;
    size_t i = 0;

    while (i < expression.size()) {
        char elem = expression[i];
        if (isspace(elem)) {
            i++;
            continue;
        }

        if (isdigit(elem) || elem == '-') {
            double value = parseNumber(expression, i);
            pushNodeFront(value, stack, stackSize);
        }
        else if (isOperator(elem)) {
            if (stackSize < 2) {
                cout << "Ошибка. Недостаточно операндов для оператора '" << elem << "' в позиции " << i << endl;
                deleteNode(stack);
                return false;
            }
            
            popNode(stack, stackSize);
            popNode(stack, stackSize);
            pushNodeFront(0.0, stack, stackSize); // Временный результат
        }
        else {
            cout << "Ошибка. Недопустимый символ '" << elem << "' в позиции " << i << endl;
            deleteNode(stack);
            return false;
        }
        i++;
    }

    if (stackSize != 1) {
        cout << "Ошибка. Неверное количество операндов в обратной польской нотации (ожидается 1, найдено " << stackSize << ")" << endl;
        deleteNode(stack);
        return false;
    }

    deleteNode(stack);
    return true;
}

bool CheckPN(string& expression) {
    if (expression.empty()) {
        cout << "Ошибка. Пустая польская нотация" << endl;
        return false;
    }

    Node* stack = nullptr;
    int stackSize = 0;

    for (int i = expression.size() - 1; i >= 0; i--) {
        char elem = expression[i];
        if (isspace(elem)) {
            continue;
        }

        if (isdigit(elem) || elem == '-') {
            size_t temp_i = i;
            string temp_str = expression;
            reverse(temp_str.begin(), temp_str.end());
            temp_i = expression.size() - 1 - temp_i;
            double value = parseNumber(temp_str, temp_i);
            i = expression.size() - 1 - temp_i;
            pushNodeFront(value, stack, stackSize);
        }
        else if (isOperator(elem)) {
            if (stackSize < 2) {
                cout << "Ошибка. Недостаточно операндов для оператора '" << elem << "' в позиции " << i << endl;
                deleteNode(stack);
                return false;
            }
            popNode(stack, stackSize);
            popNode(stack, stackSize);
            pushNodeFront(0.0, stack, stackSize); 
        }
        else {
            cout << "Ошибка. Недопустимый символ '" << elem << "' в позиции " << i << endl;
            deleteNode(stack);
            return false;
        }
    }

    if (stackSize != 1) {
        cout << "Ошибка. Неверное количество операндов в польской нотации (ожидается 1, найдено " << stackSize << ")" << endl;
        deleteNode(stack);
        return false;
    }

    deleteNode(stack);
    return true;
}

double calculateSimple(string expression) { // лень
    if (!CheckSimple(expression)) {
        cout << "Ошибка: Некорректное выражение" << endl;
        return 0.0;
    }
    string rpn = printRPN(expression);
    if (rpn.empty()) {
        cout << "Ошибка: Не удалось преобразовать выражение в RPN" << endl;
        return 0.0;
    }

    if (!CheckRPN(rpn)) {
        cout << "Ошибка: Преобразованное RPN выражение некорректно" << endl;
        return 0.0;
    }

    double result = calculateRPN(rpn);
    return result;
}

int main() {
    setlocale(0, "");
    SetConsoleCP(1251);
    string value;
    int nonationType = 1;
    int s;
    double totalResult;
    bool isStarted = true;
  while (isStarted) {
        
        cout << "\n? | Выбери действие из меню:\n";
        cout << "1 | Ввести выражение в обычном виде.\n";
        cout << "2 | Ввести выражение в польской нотации.\n";
        cout << "3 | Ввести выражение в обратной польской нотации.\n";
        cout << "4 | Проверить корректность текущего выражения.\n";
        cout << "5 | Преобразовать текущее выражение в польскую нотацию.\n";
        cout << "6 | Преобразовать текущее выражение в обратную польскую нотацию.\n";
        cout << "7 | Вычислить текущее выражение.\n";
        cout << "0 | Выйти из программы.\n";
        cin >> s;
        switch (s) {
        case 1: cin.ignore(); getline(cin, value);
            cout << "Полученное выражение: " << value << endl;
            nonationType = 1; break;
        case 2: cin.ignore(); getline(cin, value);
            cout << "Полученное выражение: (PN) " << value << endl;
            nonationType = 2; break;
        case 3: cin.ignore(); getline(cin, value);
            cout << "Полученное выражение: (RPN) " << value << endl;
            nonationType = 3; break;
        case 4: 
            if (value.empty()) {
                cout << "Необходимо ввести какон-нибудь выражение." << endl;
            }
            switch (nonationType) {
            case 1: 
                if (CheckSimple(value)) {
                    cout << "Выражение корректно!\n";
                }
                else {
                    cout << "Выражение некорректно!\n";
                }
                break;
            case 2: 
                if (CheckPN(value)) {
                    cout << "Выражение корректно!\n";
                }
                else {
                    cout << "Выражение некорректно!\n";
                }
                break;
            case 3:
                if (CheckRPN(value)) {
                    cout << "Выражение корректно!\n";
                }
                else {
                    cout << "Выражение некорректно!\n";
                }
                break;
            }
            break;
        case 5:
            if (CheckSimple(value)) {
                value = printPN(value);
                cout << "Обновлённое выражение: " << value;
            }
            break;
        case 6: 
            if (CheckSimple(value)) {
                value = printRPN(value);
                cout << "Обновлённое выражение: " << value;
            }
            break;
        case 7: 
            switch (nonationType) {
            case 1: totalResult = calculateSimple(value); break;
            case 2: totalResult = calculatePN(value); break;
            case 3: totalResult = calculateRPN(value); break;
            }
            cout << "Вычисленное выражение: " << totalResult << endl;
            break;
        case 0: isStarted = false; break;
        }  
    }
    return 0;
}