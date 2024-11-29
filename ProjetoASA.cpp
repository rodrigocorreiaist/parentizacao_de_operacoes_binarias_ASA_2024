#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <climits>
using namespace std;

// Tabela de memoização
unordered_map<string, unordered_map<int, string>> memo;

// Função auxiliar para criar a chave única para os intervalos
string makeKey(int start, int end) {
    return to_string(start) + "," + to_string(end);
}

// Função recursiva para calcular os resultados para o subarray [start, end]
unordered_map<int, string> compute(
    const vector<int>& sequence,
    const vector<vector<int>>& op_table,
    int start,
    int end
) {
    // Verifica se já calculamos este intervalo
    string key = makeKey(start, end);
    if (memo.find(key) != memo.end()) {
        return memo[key];
    }

    unordered_map<int, string> result;

    // Caso base: um único elemento no intervalo
    if (start == end) {
        result[sequence[start]] = to_string(sequence[start]);
        memo[key] = result;
        return result;
    }

    // Divide o intervalo em subintervalos e calcula os resultados
    for (int mid = start; mid < end; ++mid) {
        unordered_map<int, string> left = compute(sequence, op_table, start, mid); // Resultados do lado esquerdo
        unordered_map<int, string> right = compute(sequence, op_table, mid + 1, end); // Resultados do lado direito

        // Combina os resultados dos lados esquerdo e direito
        for (const auto& l : left) {
            for (const auto& r : right) {
                int value = op_table[l.first - 1][r.first - 1]; // Calcula o resultado usando a tabela de operações
                string expr = "(" + l.second + " " + r.second + ")";

                // Atualiza o resultado somente se for a primeira vez ou se a expressão for menor lexicograficamente
                if (result.find(value) == result.end() || expr < result[value]) {
                    result[value] = expr;
                }
            }
        }
    }

    // Armazena o resultado na memoização e retorna
    memo[key] = result;
    return result;
}

int main() {
    int n, m;

    // Lê os valores de n e m e verifica se a leitura foi bem-sucedida
    if (scanf("%d %d", &n, &m) != 2) {
        return 1;
    }

    // Lê a tabela de operações
    vector<vector<int>> op_table(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (scanf("%d", &op_table[i][j]) != 1) {
                return 1;
            }
        }
    }

    // Lê a sequência
    vector<int> sequence(m);
    for (int i = 0; i < m; ++i) {
        if (scanf("%d", &sequence[i]) != 1) {
            return 1;
        }
    }

    // Lê o valor alvo
    int target;
    if (scanf("%d", &target) != 1) {
        return 1;
    }

    // Calcula os resultados possíveis para a sequência completa
    unordered_map<int, string> results = compute(sequence, op_table, 0, m - 1);

    // Verifica se o valor alvo pode ser obtido
    if (results.find(target) != results.end()) {
        printf("1\n");
        printf("%s\n", results[target].c_str());
    } else {
        printf("0\n");
    }

    return 0;
}