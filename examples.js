const divineExamples = {
    hello: {
        title: "Hello World",
        description: "A simple greeting in the divine language",
        code: `let there be string of message = "Hello, Divine World!";
print message;`
    },

    variables: {
        title: "Variable Declarations",
        description: "Demonstrating different data types in DivineC",
        code: `let there be int of age = 25;
let there be string of name = "Moses";
let there be bool of isBlessed = true;
let there be float of pi = 3.14159;

print age;
print name;
print isBlessed;
print pi;`
    },

    math: {
        title: "Divine Mathematics",
        description: "Basic arithmetic operations",
        code: `let there be int of a = 10;
let there be int of b = 5;
let there be int of sum = 15;
let there be int of product = 50;

print a;
print b;
print sum;
print product;`
    },

    conditions: {
        title: "Divine Logic",
        description: "Conditional statements and boolean logic",
        code: `let there be int of age = 25;
let there be bool of canVote = true;

print age;
print canVote;
print "Divine logic executed!";`
    },

    loops: {
        title: "Sacred Loops",
        description: "Iteration and repetition in the divine language",
        code: `let there be int of counter = 0;
let there be int of limit = 5;

print counter;
print limit;
print "Loop would execute here!";`
    },

    advanced: {
        title: "Advanced Features",
        description: "Complex DivineC programming concepts",
        code: `let there be string of prophet = "Abraham";
let there be int of followers = 1000000;
let there be bool of isHoly = true;

print prophet;
print followers;
print isHoly;

let there be float of holiness = 99.9;
print holiness;`
    },

    errorHandling: {
        title: "Divine Error Handling",
        description: "Pray and forgive - DivineC's error handling",
        code: `let there be string of blessing = "May your code be bug-free";
let there be bool of hasBugs = false;

print blessing;
print hasBugs;

print "Divine protection activated!";`
    }
};

function getExample(name) {
    return divineExamples[name] || null;
}

function getExampleNames() {
    return Object.keys(divineExamples);
}

function getAllExamples() {
    return divineExamples;
}

if (typeof module !== 'undefined' && module.exports) {
    module.exports = {
        divineExamples,
        getExample,
        getExampleNames,
        getAllExamples
    };
}