# Лабораторная работа 9
MemoryPoolsAllocator
## Описание работы моего аллокатора
Сортировка пулов: 1й if - < n - левый плохой - return false. 2й if - тоже самое, но должны поменять с right. Далее дельты - потери памяти при взятии левого и правого пула соответственно. Если дельты совпали, то выгоднее брать пулы больших размеров, чтобы в дальнейших аллокациях иметь свободное место пулах с чанками меньшего размера. Иначе раньше ставим пул с наименьшей потерей памяти. 
В pool.h:
Вектор пар free, где first - адрес (индекс) в памяти пула свободного кусочка, second - количество чанок в этом кусочке.
1. В allocate: ищем кусочек нужной длины в free. Если нашли, то если свободная память в нем == запрашиваемая память, то удаляем этот кусочек из free, иначе прибавляем к адресу запрашиваемое кол-во чанок и убираем это же количество из доступной для записи.
2. В update_free: объединяем кусочки, если какой-то освобождается при deallocate.

Данная памятка была сделана для восстановления памяти к сдаче, так как уже прошел месяц с первого дедлайна лабораторной работы.
## Задача

Реализовать аллокатор, работающий на основе выделенных заранее блоков памяти, соответсвующий [требованиям к аллокаторам](https://en.cppreference.com/w/cpp/named_req/Allocator).

Произвести сравнение с стандарным аллокатором при взаимодействии с контейнерами, полученные результаты изобразить на графике. Например, потраченное время на добавление элемента в вектор, в зависимости от количества добавленных элементов.

Данное исследование также является частью задания и повлияет на итоговый балл.

## Принцип работы

- Для аллокации памяти аллокатор использует заранее выделенные участки памяти
- Память изначальльно выделяется [пулами](https://en.wikipedia.org/wiki/Memory_pool)
- Пулы бьются на кусочки одинакового размера
- Пул умеет выделять и освобождать память только кусочками (один или более)
- Пул обладает информацией о свободных и занятых кусочках
- Пул очищает память лишь в конце своей "жизни"
- Аллокатор при выделение памяти размер N байт ищет среди пулов наиболее подходящий так, чтобы наиболее оптимально выделять память, если это невозможно то выбрасывается исключение std::bad_alloc
- При освобождении через аллокатор, кусочки памяти вновь пригодны для использования


## Реализация

Размеры пулов, их количество, а также размеры кусочков - параметризуются (в compile time или runtime).
Аллокатор не обязательно должен владеть пулами. Дизайн на Ваше усмотрение.


## Ограничения

Запрещено пользоваться <memory_resource>


## Deadline

1. 27.03.23. 0.85
2. 03.04.23. 0.65
3. 10.04.23. 0.5


## Q&A

Q. Могут ли в разных пулах быть кусочки одинакового размера?
A. Да

Q. Как выбросить исключение?
А. throw std::bad_alloc{};
