class DivineC {
            constructor() {
                this.variables = {}
                this.output = []
            }

            tokenize(code) {
                const tokens = []
                const keywords = {
                    let: 'LET',
                    there: 'THERE',
                    be: 'BE',
                    of: 'OF',
                    blessed: 'BLESSED',
                    function: 'FUNCTION',
                    if: 'IF',
                    else: 'ELSE',
                    for: 'FOR',
                    while: 'WHILE',
                    print: 'PRINT',
                    sacred: 'SACRED',
                    divine: 'DIVINE',
                    pray: 'PRAY',
                    forgive: 'FORGIVE',
                    int: 'INT',
                    float: 'FLOAT',
                    string: 'STRING',
                    bool: 'BOOL',
                    true: 'TRUE',
                    false: 'FALSE',
                    return: 'RETURN',
                }

                const regex = /(\d+\.?\d*|"[^"]*"|[a-zA-Z_][a-zA-Z0-9_]*|[=+\-*/!<>(){};\,]|==|!=|<=|>=|&&|\|\|)/g
                let match

                while ((match = regex.exec(code)) !== null) {
                    const token = match[1]
                    if (keywords[token]) {
                        tokens.push({ type: keywords[token], value: token })
                    } else if (/^\d+\.?\d*$/.test(token)) {
                        tokens.push({ type: 'NUMBER', value: parseFloat(token) })
                    } else if (/^".*"$/.test(token)) {
                        tokens.push({ type: 'STRING', value: token.slice(1, -1) })
                    } else if (/^[a-zA-Z_][a-zA-Z0-9_]*$/.test(token)) {
                        tokens.push({ type: 'IDENTIFIER', value: token })
                    } else {
                        tokens.push({ type: 'OPERATOR', value: token })
                    }
                }

                return tokens
            }

            parse(tokens) {
                const statements = []
                let i = 0

                while (i < tokens.length) {
                    if (tokens[i].type === 'LET') {
                        const stmt = this.parseVariableDeclaration(tokens, i)
                        statements.push(stmt.statement)
                        i = stmt.nextIndex
                    } else if (tokens[i].type === 'PRINT') {
                        const stmt = this.parsePrintStatement(tokens, i)
                        statements.push(stmt.statement)
                        i = stmt.nextIndex
                    } else {
                        i++
                    }
                }

                return statements
            }

            parseVariableDeclaration(tokens, start) {
                let i = start
                if (tokens[i].type !== 'LET') return { statement: null, nextIndex: i + 1 }
                i++

                if (tokens[i]?.type !== 'THERE') return { statement: null, nextIndex: i + 1 }
                i++

                if (tokens[i]?.type !== 'BE') return { statement: null, nextIndex: i + 1 }
                i++

                const dataType = tokens[i]?.value
                i++

                if (tokens[i]?.type !== 'OF') return { statement: null, nextIndex: i + 1 }
                i++

                const name = tokens[i]?.value
                i++

                if (tokens[i]?.value !== '=') return { statement: null, nextIndex: i + 1 }
                i++

                const value = this.parseExpression(tokens, i)

                return {
                    statement: {
                        type: 'VARIABLE_DECL',
                        dataType: dataType,
                        name: name,
                        value: value.expression,
                    },
                    nextIndex: value.nextIndex,
                }
            }

            parsePrintStatement(tokens, start) {
                let i = start
                if (tokens[i].type !== 'PRINT') return { statement: null, nextIndex: i + 1 }
                i++

                const expr = this.parseExpression(tokens, i)

                return {
                    statement: {
                        type: 'PRINT',
                        expression: expr.expression,
                    },
                    nextIndex: expr.nextIndex,
                }
            }

            parseExpression(tokens, start) {
                let i = start
                const token = tokens[i]

                if (!token) return { expression: null, nextIndex: i }

                if (token.type === 'NUMBER') {
                    return { expression: { type: 'NUMBER', value: token.value }, nextIndex: i + 1 }
                } else if (token.type === 'STRING') {
                    return { expression: { type: 'STRING', value: token.value }, nextIndex: i + 1 }
                } else if (token.type === 'IDENTIFIER') {
                    return { expression: { type: 'IDENTIFIER', value: token.value }, nextIndex: i + 1 }
                } else if (token.type === 'TRUE') {
                    return { expression: { type: 'BOOLEAN', value: true }, nextIndex: i + 1 }
                } else if (token.type === 'FALSE') {
                    return { expression: { type: 'BOOLEAN', value: false }, nextIndex: i + 1 }
                }

                return { expression: null, nextIndex: i + 1 }
            }

            execute(statements) {
                this.output = []

                for (const stmt of statements) {
                    if (stmt.type === 'VARIABLE_DECL') {
                        const value = this.evaluateExpression(stmt.value)
                        this.variables[stmt.name] = value
                        this.output.push(`Created ${stmt.dataType} variable "${stmt.name}" = ${value}`)
                    } else if (stmt.type === 'PRINT') {
                        const value = this.evaluateExpression(stmt.expression)
                        this.output.push(`${value}`)
                    }
                }

                return this.output
            }

            evaluateExpression(expr) {
                if (!expr) return null

                switch (expr.type) {
                    case 'NUMBER':
                        return expr.value
                    case 'STRING':
                        return expr.value
                    case 'BOOLEAN':
                        return expr.value
                    case 'IDENTIFIER':
                        return this.variables[expr.value] || `undefined_${expr.value}`
                    default:
                        return null
                }
            }

            run(code) {
                try {
                    const tokens = this.tokenize(code)
                    const statements = this.parse(tokens)
                    const output = this.execute(statements)
                    return output.join('\n')
                } catch (error) {
                    return `Error: ${error.message}`
                }
            }
        }

        const interpreter = new DivineC()

        function runCode() {
            const code = document.getElementById('codeEditor').value
            const output = interpreter.run(code)
            document.getElementById('output').textContent = output || 'No output (Divine code executed silently).'
        }

        function clearEditor() {
            document.getElementById('codeEditor').value = ''
            document.getElementById('output').textContent = 'Welcome to DivineC, Execute your code to see the results!'
        }

        function loadExample(type) {
            const examples = {
                hello: `let there be string of message = "Hello, Divine World!";
print message;`,

                variables: `let there be int of age = 25;
let there be string of name = "Moses";
let there be bool of isBlessed = true;
let there be float of pi = 3.14159;

print age;
print name;
print isBlessed;
print pi;`,

                math: `let there be int of a = 10;
let there be int of b = 5;
let there be int of sum = 15;
let there be int of product = 50;

print a;
print b;
print sum;
print product;`,

                conditions: `let there be int of age = 25;
let there be bool of canVote = true;

print age;
print canVote;
print "Divine logic executed!";`,

                loops: `let there be int of counter = 0;
let there be int of limit = 5;

print counter;
print limit;
print "Loop would execute here!";`,
            }

            document.getElementById('codeEditor').value = examples[type] || ''
        }

        function handleKeyboardShortcuts(event) {
            if ((event.ctrlKey || event.metaKey) && event.key === 'Enter') {
                event.preventDefault()
                runCode()
            }

            if ((event.ctrlKey || event.metaKey) && event.key === 'l') {
                event.preventDefault()
                clearEditor()
            }
        }

        function initializeInterface() {
            loadExample('hello')

            document.addEventListener('keydown', handleKeyboardShortcuts)

            const runButton = document.querySelector('.btn:not(.btn-clear)')
            if (runButton) {
                runButton.addEventListener('click', runCode)
            }

            const clearButton = document.querySelector('.btn-clear')
            if (clearButton) {
                clearButton.addEventListener('click', clearEditor)
            }

            const exampleButtons = document.querySelectorAll('.example-btn')
            exampleButtons.forEach((button, index) => {
                const examples = ['hello', 'variables', 'math', 'conditions', 'loops']
                button.addEventListener('click', () => loadExample(examples[index]))
            })

            console.log('DivineC Web Interface initialized successfully!')
        }

        document.addEventListener('DOMContentLoaded', initializeInterface)