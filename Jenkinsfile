pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
                sh 'echo "Hello World from GitHub: philblum/sudoku"'
                sh '''
                    echo "Multiline shell steps works too"
                    ls -lah
                '''
            }
        }
    }
}
