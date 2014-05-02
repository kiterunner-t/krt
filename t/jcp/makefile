CLASSPATH := -cp "bin/jcip-annotations.jar:bin"


%: %.java
	javac -d bin ${CLASSPATH} $^
	java -server -cp bin $@

%: factorizer/%.java
	javac -d bin ${CLASSPATH} $^
	java -server -cp bin factorizer/$@


TestFactorizer: factorizer/FactorizerUnsafeCache.java \
    factorizer/TestFactorizer.java factorizer/Factorizer.java \
    factorizer/IFactorizerService.java


TestFactorizerService: factorizer/IFactorizerService.java \
    factorizer/FactorizerServiceStateless.java \
    factorizer/TestFactorizerService.java factorizer/Factorizer.java
	javac -d bin ${CLASSPATH} $^
	java -server -cp bin ${CLASSPATH} factorizer/$@

