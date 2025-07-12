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