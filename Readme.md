# Дипломная работа по сжатию потока видео

- [Дипломная работа по сжатию потока видео](#дипломная-работа-по-сжатию-потока-видео)
	- [Тестовые файлы](#тестовые-файлы)
	- [Сборка](#сборка)
- [Структура программы](#структура-программы)

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
mkdir build && cd build
cmake ..
make -j$(nproc)
./coding # запуск
```

# Структура программы

1. Читаем видеофайл
2. Кодируем блоки
3. Квантуем блоки
4. Записываем блоки в файл `data`
   - блоки записываем в файл подряд
5. Читаем блоки из памяти
6. Востанавливаем по ним изображение
7. Записываем в файл `.yuv` и смотрим на результат
