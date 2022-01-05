# Дипломная работа по сжатию потока видео

- [Дипломная работа по сжатию потока видео](#дипломная-работа-по-сжатию-потока-видео)
  - [Тестовые файлы](#тестовые-файлы)
  - [Сборка](#сборка)
- [Структура программы](#структура-программы)
- [Что нужно исправить](#что-нужно-исправить)
- [TODO](#todo)

## Тестовые файлы

Для тестового видео можно использовать любое видео с сайта
http://ultravideo.fi/#testsequences со следующими параметрами:

Видео Bosphorus
- Resolution: 1080p
- Bit depth: 8
- Format: YUV
- Container: RAW

Файл нужно положить в `./files/video1.yuv`. Для просмотра видео можно использовать программу
[YUView](https://github.com/IENT/YUView) (кросплатформенная).

## Сборка

Для сборки используется `cmake`.

```bash
cd build
cmake ..
make -j$(nproc)
./coding # запуск
```

# Структура программы
1. Кодирование
    - [x] Чтение кадра из файла
    - [x] Разбиение кадра на блоки 16x16
    - [x] Прямое косинусное преобразование
    - [ ] Квантование
    - [ ] Обход по зигзагу
    - [ ] Кодирование run-level
    - [ ] Кодирование пар run-level кодом Хаффмана
    - [ ] Запись результата в сжимаемый файл
2. Декодировнаие (кодирование в обратном порядке, получаем исходное видео)


# Что нужно исправить
- [ ] Двойная буфферизация (Из одного буффера пишем в файл, второй заполняется кадрами)
- [ ] CodeStyle
    - [ ] Разделение по неймспейсам
    - [ ] Сделать структуры для типов, не читать (как сделано с фреймом)
    - [ ] Избавляемся от интов в пользу uint32_t и подобных
    - [ ] Если работаем с массивами, то передаем размер и сам массив(чтобы нормально выводить без segfault) Написание высокоуровневых функций)
    - [ ] Написание большего количества абстракций
    - [ ] Глобальные счетчики на очистку и выделение памяти, сравнение памяти, чтобы счетчик не был одинаковый
- [ ] Написание тестов
- [ ] Параллельная обработка
- [ ] Написание комментарий и документации

## TODO: 
- [x] Основной цикл программы (считывание кадров и запись их в файл) 
- [x] Делаем тип блока
- [x] Делает бридж для пробросов блоков в dct
- **Сегодня**
    - [ ] Создаем контроллер для преобразования кадра в набор блоков и обратно
    - [ ] Квантование блоков
        - [ ] выбор уровня квантования
        - [ ] процесс квантования
        - [ ] процесс деквантования
