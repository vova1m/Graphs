#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <queue>
#include <QMessageBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QInputDialog>
#include <QVBoxLayout>

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), graph(0)
{
    ui->setupUi(this);


    // Создание графического виджета и добавление его на главное окно
    graphWidget = new GraphWidget(this);

    // Создание кнопки и установка текста
    breadthButton = new QPushButton("Обход в ширину", this);
    // Подключение сигнала нажатия кнопки к слоту
    connect(breadthButton, &QPushButton::clicked, this, &MainWindow::breadth);

    depthButton = new QPushButton("Обход в глубину", this);
    // Подключение сигнала нажатия кнопки к слоту
    connect(depthButton, &QPushButton::clicked, this, &MainWindow::depth);

    DijkstraButton = new QPushButton("Алгоритм Дейкстры", this);
    // Подключение сигнала нажатия кнопки к слоту
    connect(DijkstraButton, &QPushButton::clicked, this, &MainWindow::Dijkstra);

    // Создание поля для ввода числа вершин
    vertexCountLineEdit = new QLineEdit(this);
    vertexCountLineEdit->setGeometry(100,100,300,100);
    vertexCountLineEdit->setPlaceholderText("Число вершин");
    vertexCountLineEdit->setValidator(new QIntValidator(2, 100, this));

    // Подключение сигнала изменения текста в поле для ввода
    connect(vertexCountLineEdit, &QLineEdit::textChanged, this, &MainWindow::onVertexCountChanged);

    adjacencyMatrixButton = new QPushButton("Матрица смежности",this);
    connect(adjacencyMatrixButton, &QPushButton::clicked, this, &MainWindow::showAdjacencyMatrix);

    addVertexButton = new QPushButton("Добавить вершину", this);
    connect(addVertexButton, &QPushButton::clicked, this, &MainWindow::addVertex);

    addEdgeButton = new QPushButton("Добавить ребро", this);
    connect(addEdgeButton, &QPushButton::clicked, this, &MainWindow::addEdge);

    removeEdgeButton = new QPushButton("Удалить ребро", this);
    connect(removeEdgeButton, &QPushButton::clicked, this, &MainWindow::removeEdge);

    removeVertexButton = new QPushButton("Удалить вершину", this);
    connect(removeVertexButton, &QPushButton::clicked, this, &MainWindow::removeVertex);

    editEdgeWeightButton = new QPushButton("Изменить вес ребра", this);
    connect(editEdgeWeightButton, &QPushButton::clicked, this, &MainWindow::editWeight);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(vertexCountLineEdit);
    layout->addWidget(addVertexButton);
    layout->addWidget(removeVertexButton);
    layout->addWidget(addEdgeButton);
    layout->addWidget(removeEdgeButton);
    layout->addWidget(editEdgeWeightButton);
    layout->addWidget(graphWidget);
    layout->addWidget(breadthButton);
    layout->addWidget(depthButton);
    layout->addWidget(DijkstraButton);
    layout->addWidget(adjacencyMatrixButton);

    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Функция, которая вызывается при изменении числа вершин
void MainWindow::onVertexCountChanged(const QString& text)
{
    bool ok;
    int vertexCount = text.toInt(&ok);

    if (ok) {
        // Изменение графа
        updateGraph(vertexCount);
    } else {
        // Ввод некорректного значения
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите целое число вершин.");
    }
}

// Функция, которая обновляет граф при изменении числа вершин
void MainWindow::updateGraph(int vertexCount)
{
    graph = Graph(vertexCount);

    if (vertexCount < 2) {
        QMessageBox::warning(this, "Ошибка", "Число вершин должно быть не менее 2.");
        return;
    }
    bool ok = true;
    while(ok){
        int startikVertex = QInputDialog::getInt(this, "Начальная вершина", "Введите номер начальной вершины для ребра", 0, 0, vertexCount - 1, 1, &ok);
        int endVertex = QInputDialog::getInt(this, "Конечная вершина", "Введите номер конечной вершины для ребра", 0, 0, vertexCount - 1, 1, &ok);
        int weight = QInputDialog::getInt(this, "Вес ребра", "Введите вес ребра", 0, 0, std::numeric_limits<int>::max(), 1, &ok);
        graph.addEdge(startikVertex, endVertex, weight);
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Создание ребра", "Хотите создать еще одно ребро?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            ok = false;
        }
    }
    graphWidget->visGraph(graph);
}

// Функция, которая выводит матрицу смежности на экран
void MainWindow::showAdjacencyMatrix()
{
    int numVertices = graph.getNumVertices();
    std::vector<std::vector<int>> adjacencyMatrix = graph.getAdjacencyMatrix();

    QString matrixString;

    // Формирование строки с матрицей смежности
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            matrixString += QString::number(adjacencyMatrix[i][j]) + "\t";
        }
        matrixString += "\n";
    }

    // Отображение матрицы смежности в всплывающем окне
    QMessageBox::information(this, "Матрица смежности", matrixString);
}

// Функция, которая позволяет добавить вершину в граф
void MainWindow::addVertex() {
    graph.addVertex();
    updateGraph(graph.getNumVertices());
    graphWidget->visGraph(graph);
}

// Функция, которая позволяет добавить ребро в граф
void MainWindow::addEdge()
{
    int numVertices = graph.getNumVertices();
    int startikVertex = QInputDialog::getInt(this, "Начальная вершина", "Введите номер начальной вершины для ребра", 0, 0, numVertices - 1, 1);
    int endVertex = QInputDialog::getInt(this, "Конечная вершина", "Введите номер конечной вершины для ребра", 0, 0, numVertices - 1, 1);
    int weight = QInputDialog::getInt(this, "Вес ребра", "Введите вес ребра", 0, 0, std::numeric_limits<int>::max(), 1);
    graph.addEdge(startikVertex, endVertex, weight);
    graphWidget->visGraph(graph);
}

// Функция, которая позволяет удалить ребро из графа
void MainWindow::removeEdge()
{
    bool ok = true;
    int numVertices = graph.getNumVertices();
    while(ok){
        int startikVertex = QInputDialog::getInt(this, "Начальная вершина", "Введите номер начальной вершины для ребра", 0, 0, numVertices - 1, 1, &ok);
        int endVertex = QInputDialog::getInt(this, "Конечная вершина", "Введите номер конечной вершины для ребра", 0, 0, numVertices - 1, 1, &ok);
        graph.removeEdge(startikVertex, endVertex);
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Удаление ребра", "Хотите удалить еще одно ребро?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            ok = false;
        }
    }
    graphWidget->visGraph(graph);
}

// Функция, которая позволяет удалить вершину из графа
void MainWindow::removeVertex()
{
    int numVertices = graph.getNumVertices();
    int Vertex = QInputDialog::getInt(this, "Вершина", "Введите номер удаляемой вершины", 0, 0, numVertices - 1, 1);
    graph.removeVertex(Vertex);
    graphWidget->visGraph(graph);
}

// Функция, которая позволяет изменить вес ребра
void MainWindow::editWeight()
{
    int numVertices = graph.getNumVertices();
    int startikVertex = QInputDialog::getInt(this, "Начальная вершина", "Введите номер начальной вершины для ребра", 0, 0, numVertices - 1, 1);
    int endVertex = QInputDialog::getInt(this, "Конечная вершина", "Введите номер конечной вершины для ребра", 0, 0, numVertices - 1, 1);
    int weight = QInputDialog::getInt(this, "Новый вес ребра", "Введите новый вес ребра", 0, 0, std::numeric_limits<int>::max(), 1);
    graph.editEdgeWeight(startikVertex, endVertex, weight);
    graphWidget->visGraph(graph);
}

// Функция, которая позволяет вывести на экран результат обхода в ширину
void MainWindow::breadth()
{
    int numVertices = graph.getNumVertices();
    int startikVertex = QInputDialog::getInt(this, "Начальная вершина", "Введите номер начальной вершины", 0, 0, numVertices - 1, 1);
    graph.breadthFirstSearch(startikVertex);
    graphWidget->visGraph(graph);
}

// Функция, которая позволяет вывести на экран результат обхода в глубину
void MainWindow::depth()
{
    int numVertices = graph.getNumVertices();
    int startikVertex = QInputDialog::getInt(this, "Начальная вершина", "Введите номер начальной вершины", 0, 0, numVertices - 1, 1);
    graph.depthFirstSearch(startikVertex);
    graphWidget->visGraph(graph);
}

// Функция, которая позволяет вывести на экран результат алгоритма Дейкстры
void MainWindow::Dijkstra()
{
    int numVertices = graph.getNumVertices();
    int startikVertex = QInputDialog::getInt(this, "Начальная вершина", "Введите номер начальной вершины", 0, 0, numVertices - 1, 1);
    graph.Dijkstra(startikVertex, graph);
    graphWidget->visGraph(graph);
}
