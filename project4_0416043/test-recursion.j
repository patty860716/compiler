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
.method public static fact(I)I
	.limit locals 100
	.limit stack 100
ldc 0
istore 1
iload 0
ldc 1
if_icmpeq L0
iload 0
iload 0
ldc 1
isub
invokestatic aa/fact(I)I
imul
istore 1
goto L1
L0:
iload 0
istore 1
L1:
iload 1
ireturn
.end method
.method public static main([Ljava/lang/String;)V
	.limit locals 100
	.limit stack 100
	invokestatic aa/vinit()V
ldc 3
invokestatic aa/fact(I)I
putstatic aa/a I
getstatic java/lang/System/out Ljava/io/PrintStream;
getstatic aa/a I
invokestatic java/lang/String/valueOf(I)Ljava/lang/String;
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
ldc -8
putstatic aa/a I
L2:
getstatic aa/a I
ldc 5
if_icmpge L3
getstatic aa/a I
ldc 1
iadd
putstatic aa/a I
L4:
getstatic aa/a I
ldc 0
if_icmpge L5
getstatic aa/a I
ldc 1
iadd
putstatic aa/a I
getstatic java/lang/System/out Ljava/io/PrintStream;
getstatic aa/a I
invokestatic java/lang/String/valueOf(I)Ljava/lang/String;
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
goto L4
L5:
goto L2
L3:
return
.end method
