#include <cstdio>
#include <vector>
#include <unordered_set>
#include <iostream>

using namespace std;

// Estrutura para armazenar informações de cada bolinha
struct DPCellInfo {
    int value;            // O valor calculado para essa subexpressão
    int splitPoint;       // O ponto de divisão para essa subexpressão
    int leftValue;        // O valor da expressão à esquerda
    int rightValue;       // O valor da expressão à direita
};

struct DPCell {
    vector<DPCellInfo> cells; // Cada célula contém várias "bolinhas" com valores, ponto de divisão e valores à esquerda e à direita
    unordered_set<int> calculatedValues; // Para evitar valores duplicados
};

// Função para preencher a tabela DP
void fillDPTable(int m, const vector<int> &seq, const vector<vector<int>> &op, vector<vector<DPCell>> &dp, int n, int resultado) {
    dp.resize(m, vector<DPCell>(m));

    // Preencher a diagonal principal com os números da sequência
    for (int i = 0; i < m; ++i){//O(m) {
        dp[i][i].cells.push_back({seq[i], -1, -1, -1}); // A diagonal só contém o valor da sequência
        dp[i][i].calculatedValues.insert(seq[i]);
    }

    // Preencher as células acima da diagonal
    for (int length = 2; length <= m; ++length) //O(m-1) {
        for (int i = 0; i <= m - length; ++i){ //O(m -length +1) {
            int j = i + length - 1;
            for (int k = j - 1; k >= i; --k){  //O(j-i) {
                // Iteração reversa para garantir a parentização mais à esquerda
                if (dp[i][j].cells.size() == static_cast<size_t>(n)) {
                    break; // Interrompe o preenchimento se a heurística for atendida
                }
                const auto &left = dp[i][k];
                const auto &right = dp[k + 1][j];

                for (size_t v1 = 0; v1 < left.cells.size(); ++v1) {//O(n) {
                    if (dp[i][j].cells.size() == static_cast<size_t>(n)) {
                        break; // Interrompe o preenchimento se a heurística for atendida
                    }
                    for (size_t v2 = 0; v2 < right.cells.size(); ++v2){// O(n) {
                        if (dp[i][j].cells.size() == static_cast<size_t>(n)) {
                            break; // Interrompe o preenchimento se a heurística for atendida
                        }
                        int val = op[left.cells[v1].value - 1][right.cells[v2].value - 1];
                        if (dp[i][j].calculatedValues.find(val) == dp[i][j].calculatedValues.end()) {
                            dp[i][j].cells.push_back({val, k, left.cells[v1].value, right.cells[v2].value});
                            dp[i][j].calculatedValues.insert(val);
                        }

                        // Verifique se já encontramos o valor alvo
                        if (dp[0][m - 1].calculatedValues.find(resultado) != dp[0][m - 1].calculatedValues.end()) {
                            return; // Interrompe o preenchimento se já encontrarmos o valor alvo
                        }
                    }
                }
            }
        }
    }


// Função de reconstrução da expressão eficiente
void reconstructExpression(int i, int j, int target, const vector<vector<DPCell>> &dp) {
    // Encontrar a bolinha que contém o valor desejado
    const auto &cell = dp[i][j];
    for (const auto &cellInfo : cell.cells)  //O(n) {
        if (cellInfo.value == target) {
            // Se encontramos a bolinha correta, precisamos de construir a expressão
            if (i == j) {
                // Quando o intervalo é de um único número, basta retornar o número
                printf("%d", target);
            } else {
                // Caso contrário, reconstruímos a expressão com parênteses
                printf("(");
                // Recursão para a esquerda
                reconstructExpression(i, cellInfo.splitPoint, cellInfo.leftValue, dp);  
                printf(" ");
                // Recursão para a direita
                reconstructExpression(cellInfo.splitPoint + 1, j, cellInfo.rightValue, dp); 
                printf(")");
            }

            return;  // Já encontramos o valor e imprimimos a expressão, então saímos
        }
    }


// Função principal para testar a solução
int main() {
    int n, m, resultado;
    vector<vector<int>> op;
    vector<int> seq;

    // Leitura da entrada
    if (scanf("%d %d", &n, &m) != 2) exit(1);

    op.resize(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (scanf("%d", &op[i][j]) != 1) exit(1);

    seq.resize(m);
    for (int i = 0; i < m; ++i)
        if (scanf("%d", &seq[i]) != 1) exit(1);

    if (scanf("%d", &resultado) != 1) exit(1);

    vector<vector<DPCell>> dp;
    fillDPTable(m, seq, op, dp, n, resultado);

    // Verificar se o valor desejado pode ser obtido
    bool found = false;
    for (size_t v = 0; v < dp[0][m - 1].cells.size(); ++v) {
        if (dp[0][m - 1].cells[v].value == resultado) {
            found = true;
            break;
        }
    }

    // Gerar a saída
    if (found) {
        printf("1\n");
        reconstructExpression(0, m - 1, resultado, dp);
        printf("\n");
    } else {
        printf("0\n");
    }

    return 0;
}