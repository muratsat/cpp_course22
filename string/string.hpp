#pragma once

#include <cstddef>
#include <iostream>
#include <vector>

class String {
 public:
  // Конструктор по умолчанию - создает пустую строку,
  // никакой памяти не выделяется!
  String() = default;

  // Конструктор, принимающий size и character (именно в этом порядке) -
  // создает строку длины size, заполненный символами character
  String(size_t size, char character);

  // Конструктор копирования
  String(const String& orig);

  String(const char* c_string);

  // Копирующий оператор присваивания
  String& operator=(const String& other);

  // Деструктор
  ~String();

  // устанавливает размер в 0,
  // деаллокации выделенной памяти при этом НЕ происходит
  void Clear();

  // добавляет букву character в конец строки
  void PushBack(char character);

  // удаляет последнюю букву.
  // В случае пустой строки должен ничего не делать,
  // хотя для std::string это будет UB
  void PopBack();

  // изменяет размер на new_size.
  // Если вместимость не позволяет хранить столько
  // символов, то выделяется новый буфер с вместимостью new_size.
  void Resize(size_t new_size);

  // то же, что и Resize(new_size),
  // но в случае new_size > size
  // заполняет недостающие элементы значением character.
  void Resize(size_t new_size, char character);

  // изменяет вместимость на max(new_cap, текущая вместимость)
  // (если new_cap <= текущая вместимость, то делать ничего не нужно).
  // Размер при этом не изменяется.
  void Reserve(size_t new_cap);

  // уменьшает capacity до size (если capacity > size)
  void ShrinkToFit();

  // обменивает содержимое с другой строкой other. Должен работать за O(1)
  void Swap(String& other);

  // Константный оператор доступа по индексу[]
  const char& operator[](size_t id) const;

  // Неконстантный оператор доступа по индексу[]
  // Неконстантный должен позволять изменять полученный элемент(a[1] = 5)
  char& operator[](size_t id);

  // Константный доступ к первому символам
  char& Front();

  // Неконстантный доступ к первому символам
  char Front() const;

  // Неконстантный доступ к последнему символам
  char& Back();

  // Константный доступ к последнему символам
  char Back() const;

  // true, если строка пустая (размер 0)
  bool Empty() const;

  // возвращает размер
  size_t Size();

  // возвращает вместимость
  size_t Capacity();

  // возвращает указатель на начало массива.
  char* Data();

  // Операторы сравнения (<, >, <=, >=, ==, !=),
  // задающие лексикографический порядок
  friend bool operator<(const String& s1, const String& s2);
  friend bool operator>(const String& s1, const String& s2);
  friend bool operator<=(const String& s1, const String& s2);
  friend bool operator>=(const String& s1, const String& s2);
  friend bool operator==(const String& s1, const String& s2);
  friend bool operator!=(const String& s1, const String& s2);

  // Оператор + для конкатенации строк.
  // Например, "ab" + "oba" = "aboba".
  String& operator+(const String& other) const;

  // Оператор += для конкатенации строк.
  // Операция s += t должна работать за O(|t|)!!!
  // Иначе не дождетесь итогов тестирования и спалите тестирующие сервера
  String& operator+=(const String& other);

  // Оператор умножения на число.
  // Принимает строку str и число n (именно в таком порядке)
  // и вернет строку вида str + ... + str (n раз).
  // Сложность по времени должна быть O(длины результата).
  // Умножение числа на строку должно приводить ошибку компиляции!
  // Загадка о лектора по алгоритмам - не возникло ли у вас
  // аналогии с каким-то известным вам алгоритмом?)
  String operator*(int n) const;

  // Оператор ввода из потока
  friend std::istream& operator>>(std::istream& input, String& s);

  // Оператор вывода в поток.
  friend std::ostream& operator<<(std::ostream& output, const String& s);

  // Аналог сплита в питоне.
  std::vector<String> Split(const String& delim = " ");

  // Аналог джоина в питоне.
  String Join(const std::vector<String>& strings);

 private:
  char* s_ = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 0;
};