#!/usr/bin/env python3

# Leer el archivo
with open('expression.hpp', 'r') as f:
    lines = f.readlines()

# Clases que necesitan evaluate() pero no lo tienen
classes_needing_evaluate = [
    'NameExpression', 'IntExpression', 'StrExpression', 'RealExpression', 
    'BooleanExpression', 'ArrayExpression', 'SubscriptExpression'
]

# Buscar cada clase y agregar evaluate() antes del último }
for i, line in enumerate(lines):
    for class_name in classes_needing_evaluate:
        if f'class {class_name}' in line:
            # Buscar el final de la clase
            brace_count = 0
            j = i
            while j < len(lines):
                if '{' in lines[j]:
                    brace_count += lines[j].count('{')
                if '}' in lines[j]:
                    brace_count -= lines[j].count('}')
                    if brace_count == 0:
                        # Encontramos el final de la clase
                        # Verificar si ya tiene evaluate()
                        class_content = ''.join(lines[i:j+1])
                        if 'std::string evaluate() const noexcept override;' not in class_content:
                            # Agregar evaluate() antes del cierre
                            lines.insert(j, '    std::string evaluate() const noexcept override;\n')
                        break
                j += 1
            break

# Escribir el archivo modificado
with open('expression.hpp', 'w') as f:
    f.writelines(lines)

print("Declaraciones de evaluate() agregadas exitosamente")
