# skip_list
Разработаны классы slmap и slmultimap, отвечающие функциональности классов std::map и std::multimap, 
реализованные на основнии структуры данных skip list (W. Pugh "Skip Lists: A Probabilistic Alternative to
Balanced Trees", Communications of the ACM, v. 33, n. 6, p. 668-676, 1990 ).

В заголовочном файле slmap.h  реализован ассоциативный массив. 
В slstruct.h реализована структура данных список с пропусками, на основе которой реализован ассоциативный массив.
В фале main.cpp приведен пример использования разработанных классов.
