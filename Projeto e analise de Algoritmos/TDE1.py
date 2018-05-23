class Pessoa:
  def __init__(self, nome, tarefa, peso):
    self.nome, self.tarefa, self.peso =  nome, tarefa, peso
  def __repr__(self):
    return self.nome + ' '  + self.tarefa

pessoas = [Pessoa('p1', 'tarefa 1', 9), Pessoa('p1', 'tarefa 2', 2), Pessoa('p1', 'tarefa 3',7), Pessoa('p1', 'tarefa 4', 8),

 Pessoa('pe2', 'tarefa 1',6 ),Pessoa('pe2', 'tarefa 2', 4),Pessoa('pe2', 'tarefa 3', 3),Pessoa('pe2', 'tarefa 4', 7),  
 
 Pessoa('pes3', 'tarefa 1', 5),Pessoa('pes3', 'tarefa 2', 8),Pessoa('pes3', 'tarefa 3', 1),Pessoa('pes3', 'tarefa 4', 8),

 Pessoa('pess', 'tarefa 1', 7), Pessoa('pess', 'tarefa 2', 6), Pessoa('pess', 'tarefa 3', 9), Pessoa('pess', 'tarefa 4', 4)]

stack = []
possibilities = []

def opa(stack):
  for i in pessoas:
    if len(stack) == 4:
      possibilities.append(stack)
      stack.pop()
    if i.nome in [k.nome for k in stack]:continue
    if i.tarefa in [k.tarefa for k in stack]: continue
    else:
      stack.append(i)
    opa(stack)
    
for pessoa in pessoas:
  stack.append(pessoa)
  opa(stack)
  stack = []
print(possibilities)
