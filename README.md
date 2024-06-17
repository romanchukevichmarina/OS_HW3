# Романчукевич Марина ИДЗ-3

### Вариант 29 -- Задача про экзамен
Преподаватель проводит экзамен у группы студентов. Каждый студент получает свой билет, сообщает его номер и готовит письменный ответ за некоторое случайное время. Подготовив ответ, он передает его преподавателю. Преподаватель некоторое случайное время просматривает ответ и сообщает студенту оценку.
Требуется создать клиент–серверное приложение, моделирующее действия преподавателя и студентов.
Преподаватель — сервер. Каждый студент — отдельный клиент. Для автоматизации запуска студентов можно использовать скрипт, организующий поступление клиентов со случайной задержкой. Необходимо учесть, что одновременно сдавать экзамен может несколько студентов. То есть, сервер должен обслуживать более одного клиента.

### 4–5 баллов
Разработать клиент–серверное приложение, в котором сервер (или серверы) и клиенты независимо друг от друга отобра- жают только ту информацию, которая поступает им во время обмена. То есть, отсутствует какой-либо общий вывод интегрированной информации, отображающий поведение системы в целом.

Первым запускается сервер, затем преподаватель и только потом студенты. Для преподавателя и студентов предназначены разные порты. При запуске сервера параметры выклядят так: "<Порт для студентов> <Порт для преподавателя>". 
При запуске студентов и преподавателя в качестве аргументов передаются IP, а затем соответствующий порт. Если студента запустить раньше преподаватедя, то он уйдет, не дождавшись экзамена. Сервер сначала ожидает прихода преподавателя, а затем уже начинает ждать студентов. Когда студент приходит, он посылает серверу свой pid, студент становится в очередь на сдачу. Потом сервер пересылает pid студента преподавателю, тот ждет 0-9 секунд, отправляет pid обратно, сервер принимает и пересылает студенту, что означает, что студент сдал экзамен. По окончании работы каждой программы происходит закрытие сокетов.
