#!/usr/bin/env python3

import re

# Leer el archivo expression.hpp
with open('expression.hpp', 'r') as f:
    content = f.read()

# Lista de clases que necesitan evaluate()
classes_needing_evaluate = [
    'NotExpression', 'OrExpression', 'XorExpression', 'SubExpression', 'DivExpression', 'ModExpression',
    'LessExpression', 'LessEqExpression', 'GreaterExpression', 'GreaterEqExpression', 
    'EqualExpression', 'NotEqualExpression', 'TailExpression', 'PrintExpression',
    'ItorExpression', 'RtoiExpression', 'AddArrayExpression', 'DelArrayExpression',
    'ArgExpression', 'CallExpression', 'FunctionExpression'
]

# Para cada clase, agregar la declaración de evaluate()
for class_name in classes_needing_evaluate:
    # Buscar la clase y agregar evaluate() antes del último }
    pattern = r'(class ' + class_name + r'[^{{]*\{[^}}]*)(\s*\};\s*)'
    
    def add_evaluate(match):
        class_content = match.group(1)
        closing = match.group(2)
        
        # Verificar si ya tiene evaluate()
        if 'std::string evaluate() const noexcept override;' in class_content:
            return match.group(0)
        
        # Agregar evaluate() antes del cierre
        return class_content + '\n    std::string evaluate() const noexcept override;\n' + closing
    
    content = re.sub(pattern, add_evaluate, content, flags=re.DOTALL)

# Escribir el archivo modificado
with open('expression.hpp', 'w') as f:
    f.write(content)

print("Declaraciones de evaluate() agregadas exitosamente")
