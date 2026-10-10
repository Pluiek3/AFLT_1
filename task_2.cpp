#include <iostream>
#include <string>
#include <cctype>
using namespace std;

const char lhs[9]   = {' ', 'S', 'S', 'S', 'A', 'A', 'A', 'B', 'B'};
const string rhs[9] = {"", "Aba", "bS", "cAbA", "AaS", "BaB", "e", "b", "aA"};

bool isNonterminal(char c)
{
    return isupper(static_cast<unsigned char>(c)) != 0;
}

string applyRule(const string& form, int pos, int rule)
{
    return form.substr(0, pos) + rhs[rule] + form.substr(pos + 1);
}

int findLeftmostNonterminal(const string& form)
{
    for (int i = 0; i < (int)form.size(); ++i)
        if (isNonterminal(form[i]))
            return i;
    return -1;
}

int findFirstStar(const string& lsf)
{
    size_t pos = lsf.find('*');
    if (pos == string::npos)
        return -1;
    return (int)pos;
}

string expandRuleForLsf(char nonTerm, int rule)
{
    string repl = string(1, nonTerm) + "(" + to_string(rule) + ")(";
    const string& r = rhs[rule];

    for (int i = 0; i < (int)r.size(); ++i)
    {
        if (i > 0)
            repl += " ";

        char c = r[i];

        if (c == 'e')
            repl += "ε";
        else if (isNonterminal(c))
        {
            repl += '*';
            repl += c;
        }
        else
            repl += c;
    }

    repl += ")";
    return repl;
}

string updateLsf(const string& lsf, char nonTerm, int rule)
{
    int starPos = findFirstStar(lsf);
    if (starPos < 0)
        return lsf;

    string repl = expandRuleForLsf(nonTerm, rule);

    return lsf.substr(0, starPos) + repl + lsf.substr(starPos + 2);
}

void printSequence(const int* seq, int n)
{
    cout << "Последовательность: ";
    for (int i = 0; i < n; ++i)
        cout << seq[i] << " ";
}

void printStep(const string& form, int pos, int rule)
{
    cout << " => ";

    for (int i = 0; i < (int)form.size(); ++i)
    {
        if (i == pos)
            cout << form[i] << "(" << rule << ")";
        else if (form[i] == 'e')
            cout << "ε";
        else
            cout << form[i];
    }
}

bool hasNonterminals(const string& form)
{
    for (char c : form)
        if (isNonterminal(c))
            return true;
    return false;
}

void printFinal(const string& form, const string& lsf)
{
    cout << "\nЦепочка: ";
    for (char c : form)
        if (c != 'e')
            cout << c;

    cout << "\nЛСФ: " << lsf << "\n";
}

int process(const int* seq, int n)
{
    string form = "S";
    string lsf  = "*S";

    printSequence(seq, n);
    cout << "\nS";

    for (int step = 0; step < n; ++step)
    {
        int rule = seq[step];

        int pos = findLeftmostNonterminal(form);
        if (pos < 0)
        {
            cout << "\nОшибка: нет нетерминалов\n";
            return 0;
        }

        if (form[pos] != lhs[rule])
        {
            cout << "\nОшибка: правило " << rule
                 << " не применимо к " << form[pos] << "\n";
            return 0;
        }

        printStep(form, pos, rule);
        lsf  = updateLsf(lsf, form[pos], rule);
        form = applyRule(form, pos, rule);
    }

    if (hasNonterminals(form))
    {
        cout << "\nОшибка: остались нетерминалы\n";
        return 0;
    }

    printFinal(form, lsf);
    return 1;
}

int main()
{
    int seq1[] = {2, 2, 2, 1, 4, 5, 7, 8, 6, 1, 6};
    int seq2[] = {2, 2, 2, 1, 5, 7, 8, 4, 6, 1, 6};

    cout << "Последовательность 1\n";
    process(seq1, 11);

    cout << "\nПоследовательность 2\n";
    process(seq2, 11);

    return 0;
}
