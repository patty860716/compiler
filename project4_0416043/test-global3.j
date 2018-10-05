.class public aa
.super java/lang/Object
.field public static a I
.field public static b I
.field public static c F
.method public static vinit()V
	.limit locals 100
	.limit stack 100
	ldc 0
	putstatic aa/a I
	ldc 0
	putstatic aa/b I
	ldc 0.0
	putstatic aa/c F
	return
.end method
.method public <init>()V
	aload_0
	invokenonvirtual java/lang/Object/<init>()V
	return
.end method
.method public static tt()I
	.limit locals 100
	.limit stack 100
bipush 2
multianewarray [I 1
astore 0
bipush 2
multianewarray [I 1
astore 1
ldc 0
istore 2
aload 0
ldc 1
ldc 1
isub
ldc 3
iastore
aload 1
ldc 1
ldc 1
isub
aload 0
ldc 1
ldc 1
isub
iaload
ldc 4
iadd
iastore
ldc 4.4
putstatic aa/c F
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "aaa"
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
aload 1
ldc 1
ldc 1
isub
iaload
istore 2
iload 2
ireturn
.end method
.method public static main([Ljava/lang/String;)V
	.limit locals 100
	.limit stack 100
	invokestatic aa/vinit()V
invokestatic aa/tt()I
putstatic aa/a I
ldc 3
putstatic aa/a I
ldc 4
putstatic aa/b I
return
.end method
