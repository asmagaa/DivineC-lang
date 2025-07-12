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