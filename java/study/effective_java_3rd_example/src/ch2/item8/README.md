# 8. finalizer와 cleaner의 사용을 피하라
- 자바는 두 가지 객체 소멸자를 제공한다. 그 중 finalizer는 예측할 수 없고, 상황에 따라 위험할 수 있어 일반적으로 불필요하다.  
- 오동작, 낮은 성능, 이식성 문제의 원인이 되기도 한다. 나름의 쓰임새가 몇 가지 있긴 하지만 기본적으로 쓰지말아야 한다.

<br>  

## finalizer와 cleaner 사용하면 안되는 이유
### 1. finalizer와 cleaner는 제때 실행되어야 하는 작업은 할 수 없다.
finalizer와 cleaner는 즉시 수행된다는 보장이 없다. 객체에 접근할 수 없게 된 후 finalizer나 cleaner가 실행되기 까지 얼마나 걸릴지 알 수 없다.

#### 예방법
- 자바 언어 명세는 어떤 스레드가 finalizer를 수행하지 명시하지 않으니 이 문제를 예방할 보편적인 방법은 finalizer를 사용하지 않는 수 밖에 없다. 
- cleaner는 자신을 수행할 스레드를 제어할 수 있지만 여전히 백그라운드에서 수행되며 가비지 컬렉터의 통제하에 있으니 즉각 수행되리라는 보장은 없다. 
- 따라서 상태를 영구적으로 수정하는 작업에서는 절대 finalizer니 cleaner에 의존해서는 안 된다.
- System.gc나 System.runFinalization에 현혹되지 말자.

<br>

### 2.finalizer 동작 중 발생한 예외는 무시되며, 처리할 작업이 남았더라도 그 순간 종료된다.
- 잡지 못한 예외 때문에 해당 객체는 자칫 마무리가 덜 된 상태로 남을 수 있다. 그리고 다른 스레드가 이처럼 훼손된 객체를 사용하려 한다면 어떻게 동작할지 예측할 수 없다.
- 그나마 cleaner를 사용하는 라이브러리는 자신의 스레드를 통제하기 때문에 이러한 문제가 발생하지 않는다.

<br>

### 3. finalizer와 cleaner는 심각한 성능 문제도 동반한다.
- 간단한 AutoCloseable 객체를 생성하고 가비지 컬렉터가 수거하기까지 12ns가 걸린 반면(try-with-resources로 자신을 닫도록했다,) finalizer를 사용하면 약 50배 정도 차이가 나는 500ns가 걸렸고, cleaner도 비슷하게 걸렸다고 한다.
- 안정망 형태로 사용하면 훨씬 빨라진다. 약 66ns정도로 5배 정도만 느려진다.

<br>

### 4. finalizer를 사용한 클래스는 finalizer 공격에 노출되어 심각한 보안 문제를 일으킬 수도 있다.
- finalizer의 공격 원리는 간단하다. 생성자나 직렬화 과정(readObject와 readResolve)에서 예외가 발생하면, 이 생성되다 만 객체에서 악의적인 하위 클래스의 finalizer가 수행될 수 있게 된다.
- 이 finalizer는 정적 필드에 자신의 참조를 할당하여 가비지 컬렉터가 수집하지 못하게 막을 수 있다. 이렇게 일그러진 객체가 만들어지고 나면, 이 객체의 메서드를 호출해 애초에는 허용되지 않았을 작업을 수행하는 건 일도 아니다.
- 객체 생성을 막으려면 생성자에서 예외를 던지는 것만으로 충분하지만, finalizer가 있다면 그렇지도 않다.  

이러한 공격은 끔직한 결과를 초래한다.
- final 클래스들은 그 누구도 하위 클래스를 만들 수 없으니 이 공격에 안전하다.
- final이 아닌 클래스를 finalizer 공격으로부터 방어하려면 아무 일도 하지 않는 finalize 메서드를 만들고 final을 선언하자.
- finalize 공격 예제는 [finalize_attack](https://github.com/loosie/java_practice/tree/master/effective_java_3rd_example/src/ch2.item8/finalize_attack)에 있습니다.

<br>

## 대신 AutoCloseable을 구현하여 close 메서드를 사용하자.
- 파일이나 스레드 등 종료해야 할 자원을 담고 있는 객체의 클래스에서 finalizer와 cleaner를 대신해서 AutoCloseable을 구현해주고, 클라이언트에서 인스턴스를 다 쓰고 나면 close 메서드를 호출하면 된다. 
- 일반적으로 예외가 발생해도 제대로 종료되도록 try-with-resources를 사용해야 한다.

<br>

## finalizer와 cleaner도 쓰일 구석이 있긴 있다.
### 1. 자원의 소유자가 close메서드를 호출하지 않는 것에 대비한 안정만 역할로 사용하자.
- cleaner나 finalizer가 즉시 (혹은 끝까지) 호출되리라는 보장은 없지만,  클라이언트가 하지 않은 자원 회수를 늦게라도 해주는 것이 아예 안하는 것보다는 낫다. 물론 이런 finalizer를 작성할 때는 그만한 값어치가 있는지 심사숙고하자.

<br>

### 2. 네이티브 피어(native peer)와 연결된 객체에 활용하자.
- 네이티브 피어란 일반 자바 객체가 네이브트 메서드를 통해 기능을 위임한 네이티브 객체를 말한다. 네이티브 피어는 자바 객체가 아니니 가비지 컬렉터는 그 존재를 알지 못한다. 그 결과 자바 피어를 회수할 때 네이티브 객체까지 회수하지 못한다.
- cleaner나 finalizer가 나서서 처리하기 적당한 작업이다. 단, 성능 저하를 감당할 수 있고 네이티브 피어가 심각한 자원을 가지고 있지 않을 때에만 해당된다. 성능 저하를 감당할 수 없거나 네이티브 피어가 사용하는 자원을 즉시 회수해야 한다면 앞서 설명한 close메서드를 사용해야 한다.

<br> 
 
## 핵심 정리 
cleaner(자바 8까지는 finalizer)는 안전망 역할이나 중요하지 않은 네이티브 자원 회수용으로만 사용하자. 물론 이런 경우라도 불확실성과 성능 저하에 주의해야 한다.
 
--- 


💡 본문은 [개발 블로그](https://loosie.tistory.com/598) 에 있습니다.
