
a.out:     file format elf32-i386


Disassembly of section .init:

080482c0 <_init>:
 80482c0:	55                   	push   %ebp
 80482c1:	89 e5                	mov    %esp,%ebp
 80482c3:	53                   	push   %ebx
 80482c4:	83 ec 04             	sub    $0x4,%esp
 80482c7:	e8 00 00 00 00       	call   80482cc <_init+0xc>
 80482cc:	5b                   	pop    %ebx
 80482cd:	81 c3 d4 14 00 00    	add    $0x14d4,%ebx
 80482d3:	8b 93 fc ff ff ff    	mov    -0x4(%ebx),%edx
 80482d9:	85 d2                	test   %edx,%edx
 80482db:	74 05                	je     80482e2 <_init+0x22>
 80482dd:	e8 1e 00 00 00       	call   8048300 <__gmon_start__@plt>
 80482e2:	e8 e9 00 00 00       	call   80483d0 <frame_dummy>
 80482e7:	e8 44 03 00 00       	call   8048630 <__do_global_ctors_aux>
 80482ec:	58                   	pop    %eax
 80482ed:	5b                   	pop    %ebx
 80482ee:	c9                   	leave  
 80482ef:	c3                   	ret    

Disassembly of section .plt:

080482f0 <__gmon_start__@plt-0x10>:
 80482f0:	ff 35 a4 97 04 08    	pushl  0x80497a4
 80482f6:	ff 25 a8 97 04 08    	jmp    *0x80497a8
 80482fc:	00 00                	add    %al,(%eax)
	...

08048300 <__gmon_start__@plt>:
 8048300:	ff 25 ac 97 04 08    	jmp    *0x80497ac
 8048306:	68 00 00 00 00       	push   $0x0
 804830b:	e9 e0 ff ff ff       	jmp    80482f0 <_init+0x30>

08048310 <putchar@plt>:
 8048310:	ff 25 b0 97 04 08    	jmp    *0x80497b0
 8048316:	68 08 00 00 00       	push   $0x8
 804831b:	e9 d0 ff ff ff       	jmp    80482f0 <_init+0x30>

08048320 <__libc_start_main@plt>:
 8048320:	ff 25 b4 97 04 08    	jmp    *0x80497b4
 8048326:	68 10 00 00 00       	push   $0x10
 804832b:	e9 c0 ff ff ff       	jmp    80482f0 <_init+0x30>

08048330 <printf@plt>:
 8048330:	ff 25 b8 97 04 08    	jmp    *0x80497b8
 8048336:	68 18 00 00 00       	push   $0x18
 804833b:	e9 b0 ff ff ff       	jmp    80482f0 <_init+0x30>

Disassembly of section .text:

08048340 <_start>:
 8048340:	31 ed                	xor    %ebp,%ebp
 8048342:	5e                   	pop    %esi
 8048343:	89 e1                	mov    %esp,%ecx
 8048345:	83 e4 f0             	and    $0xfffffff0,%esp
 8048348:	50                   	push   %eax
 8048349:	54                   	push   %esp
 804834a:	52                   	push   %edx
 804834b:	68 c0 85 04 08       	push   $0x80485c0
 8048350:	68 d0 85 04 08       	push   $0x80485d0
 8048355:	51                   	push   %ecx
 8048356:	56                   	push   %esi
 8048357:	68 fb 84 04 08       	push   $0x80484fb
 804835c:	e8 bf ff ff ff       	call   8048320 <__libc_start_main@plt>
 8048361:	f4                   	hlt    
 8048362:	90                   	nop
 8048363:	90                   	nop
 8048364:	90                   	nop
 8048365:	90                   	nop
 8048366:	90                   	nop
 8048367:	90                   	nop
 8048368:	90                   	nop
 8048369:	90                   	nop
 804836a:	90                   	nop
 804836b:	90                   	nop
 804836c:	90                   	nop
 804836d:	90                   	nop
 804836e:	90                   	nop
 804836f:	90                   	nop

08048370 <__do_global_dtors_aux>:
 8048370:	55                   	push   %ebp
 8048371:	89 e5                	mov    %esp,%ebp
 8048373:	53                   	push   %ebx
 8048374:	83 ec 04             	sub    $0x4,%esp
 8048377:	80 3d c4 97 04 08 00 	cmpb   $0x0,0x80497c4
 804837e:	75 3f                	jne    80483bf <__do_global_dtors_aux+0x4f>
 8048380:	a1 c8 97 04 08       	mov    0x80497c8,%eax
 8048385:	bb c4 96 04 08       	mov    $0x80496c4,%ebx
 804838a:	81 eb c0 96 04 08    	sub    $0x80496c0,%ebx
 8048390:	c1 fb 02             	sar    $0x2,%ebx
 8048393:	83 eb 01             	sub    $0x1,%ebx
 8048396:	39 d8                	cmp    %ebx,%eax
 8048398:	73 1e                	jae    80483b8 <__do_global_dtors_aux+0x48>
 804839a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 80483a0:	83 c0 01             	add    $0x1,%eax
 80483a3:	a3 c8 97 04 08       	mov    %eax,0x80497c8
 80483a8:	ff 14 85 c0 96 04 08 	call   *0x80496c0(,%eax,4)
 80483af:	a1 c8 97 04 08       	mov    0x80497c8,%eax
 80483b4:	39 d8                	cmp    %ebx,%eax
 80483b6:	72 e8                	jb     80483a0 <__do_global_dtors_aux+0x30>
 80483b8:	c6 05 c4 97 04 08 01 	movb   $0x1,0x80497c4
 80483bf:	83 c4 04             	add    $0x4,%esp
 80483c2:	5b                   	pop    %ebx
 80483c3:	5d                   	pop    %ebp
 80483c4:	c3                   	ret    
 80483c5:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 80483c9:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

080483d0 <frame_dummy>:
 80483d0:	55                   	push   %ebp
 80483d1:	89 e5                	mov    %esp,%ebp
 80483d3:	83 ec 18             	sub    $0x18,%esp
 80483d6:	a1 c8 96 04 08       	mov    0x80496c8,%eax
 80483db:	85 c0                	test   %eax,%eax
 80483dd:	74 12                	je     80483f1 <frame_dummy+0x21>
 80483df:	b8 00 00 00 00       	mov    $0x0,%eax
 80483e4:	85 c0                	test   %eax,%eax
 80483e6:	74 09                	je     80483f1 <frame_dummy+0x21>
 80483e8:	c7 04 24 c8 96 04 08 	movl   $0x80496c8,(%esp)
 80483ef:	ff d0                	call   *%eax
 80483f1:	c9                   	leave  
 80483f2:	c3                   	ret    
 80483f3:	90                   	nop

080483f4 <varg>:
 80483f4:	55                   	push   %ebp
 80483f5:	89 e5                	mov    %esp,%ebp
 80483f7:	83 ec 28             	sub    $0x28,%esp
 80483fa:	8d 45 08             	lea    0x8(%ebp),%eax
 80483fd:	83 c0 04             	add    $0x4,%eax
 8048400:	89 45 ec             	mov    %eax,-0x14(%ebp)
 8048403:	c7 45 e8 00 00 00 00 	movl   $0x0,-0x18(%ebp)
 804840a:	e9 cb 00 00 00       	jmp    80484da <varg+0xe6>
 804840f:	8b 45 e8             	mov    -0x18(%ebp),%eax
 8048412:	83 f8 01             	cmp    $0x1,%eax
 8048415:	74 44                	je     804845b <varg+0x67>
 8048417:	83 f8 01             	cmp    $0x1,%eax
 804841a:	7f 09                	jg     8048425 <varg+0x31>
 804841c:	85 c0                	test   %eax,%eax
 804841e:	74 14                	je     8048434 <varg+0x40>
 8048420:	e9 b1 00 00 00       	jmp    80484d6 <varg+0xe2>
 8048425:	83 f8 02             	cmp    $0x2,%eax
 8048428:	74 58                	je     8048482 <varg+0x8e>
 804842a:	83 f8 03             	cmp    $0x3,%eax
 804842d:	74 7b                	je     80484aa <varg+0xb6>
 804842f:	e9 a2 00 00 00       	jmp    80484d6 <varg+0xe2>
 8048434:	83 45 ec 04          	addl   $0x4,-0x14(%ebp)
 8048438:	8b 45 ec             	mov    -0x14(%ebp),%eax
 804843b:	83 e8 04             	sub    $0x4,%eax
 804843e:	0f b6 00             	movzbl (%eax),%eax
 8048441:	88 45 f0             	mov    %al,-0x10(%ebp)
 8048444:	0f be 55 f0          	movsbl -0x10(%ebp),%edx
 8048448:	b8 80 86 04 08       	mov    $0x8048680,%eax
 804844d:	89 54 24 04          	mov    %edx,0x4(%esp)
 8048451:	89 04 24             	mov    %eax,(%esp)
 8048454:	e8 d7 fe ff ff       	call   8048330 <printf@plt>
 8048459:	eb 7b                	jmp    80484d6 <varg+0xe2>
 804845b:	83 45 ec 04          	addl   $0x4,-0x14(%ebp)
 804845f:	8b 45 ec             	mov    -0x14(%ebp),%eax
 8048462:	83 e8 04             	sub    $0x4,%eax
 8048465:	0f b6 00             	movzbl (%eax),%eax
 8048468:	88 45 f1             	mov    %al,-0xf(%ebp)
 804846b:	0f be 55 f1          	movsbl -0xf(%ebp),%edx
 804846f:	b8 80 86 04 08       	mov    $0x8048680,%eax
 8048474:	89 54 24 04          	mov    %edx,0x4(%esp)
 8048478:	89 04 24             	mov    %eax,(%esp)
 804847b:	e8 b0 fe ff ff       	call   8048330 <printf@plt>
 8048480:	eb 54                	jmp    80484d6 <varg+0xe2>
 8048482:	83 45 ec 04          	addl   $0x4,-0x14(%ebp)
 8048486:	8b 45 ec             	mov    -0x14(%ebp),%eax
 8048489:	83 e8 04             	sub    $0x4,%eax
 804848c:	0f b7 00             	movzwl (%eax),%eax
 804848f:	66 89 45 f2          	mov    %ax,-0xe(%ebp)
 8048493:	0f bf 55 f2          	movswl -0xe(%ebp),%edx
 8048497:	b8 8f 86 04 08       	mov    $0x804868f,%eax
 804849c:	89 54 24 04          	mov    %edx,0x4(%esp)
 80484a0:	89 04 24             	mov    %eax,(%esp)
 80484a3:	e8 88 fe ff ff       	call   8048330 <printf@plt>
 80484a8:	eb 2c                	jmp    80484d6 <varg+0xe2>
 80484aa:	83 45 ec 04          	addl   $0x4,-0x14(%ebp)
 80484ae:	8b 45 ec             	mov    -0x14(%ebp),%eax
 80484b1:	83 e8 04             	sub    $0x4,%eax
 80484b4:	8b 00                	mov    (%eax),%eax
 80484b6:	89 45 f4             	mov    %eax,-0xc(%ebp)
 80484b9:	8b 45 f4             	mov    -0xc(%ebp),%eax
 80484bc:	8b 10                	mov    (%eax),%edx
 80484be:	b8 9e 86 04 08       	mov    $0x804869e,%eax
 80484c3:	89 54 24 08          	mov    %edx,0x8(%esp)
 80484c7:	8b 55 f4             	mov    -0xc(%ebp),%edx
 80484ca:	89 54 24 04          	mov    %edx,0x4(%esp)
 80484ce:	89 04 24             	mov    %eax,(%esp)
 80484d1:	e8 5a fe ff ff       	call   8048330 <printf@plt>
 80484d6:	83 45 e8 01          	addl   $0x1,-0x18(%ebp)
 80484da:	8b 45 08             	mov    0x8(%ebp),%eax
 80484dd:	39 45 e8             	cmp    %eax,-0x18(%ebp)
 80484e0:	0f 8c 29 ff ff ff    	jl     804840f <varg+0x1b>
 80484e6:	c7 45 ec 00 00 00 00 	movl   $0x0,-0x14(%ebp)
 80484ed:	c7 04 24 0a 00 00 00 	movl   $0xa,(%esp)
 80484f4:	e8 17 fe ff ff       	call   8048310 <putchar@plt>
 80484f9:	c9                   	leave  
 80484fa:	c3                   	ret    

080484fb <main>:
 80484fb:	55                   	push   %ebp
 80484fc:	89 e5                	mov    %esp,%ebp
 80484fe:	83 e4 f0             	and    $0xfffffff0,%esp
 8048501:	53                   	push   %ebx
 8048502:	83 ec 3c             	sub    $0x3c,%esp
 8048505:	66 c7 44 24 2c 03 00 	movw   $0x3,0x2c(%esp)
 804850c:	c6 44 24 2e 61       	movb   $0x61,0x2e(%esp)
 8048511:	c6 44 24 2f 62       	movb   $0x62,0x2f(%esp)
 8048516:	c7 44 24 28 04 00 00 	movl   $0x4,0x28(%esp)
 804851d:	00 
 804851e:	c7 04 24 00 00 00 00 	movl   $0x0,(%esp)
 8048525:	e8 ca fe ff ff       	call   80483f4 <varg>
 804852a:	0f be 44 24 2e       	movsbl 0x2e(%esp),%eax
 804852f:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048533:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
 804853a:	e8 b5 fe ff ff       	call   80483f4 <varg>
 804853f:	0f be 54 24 2f       	movsbl 0x2f(%esp),%edx
 8048544:	0f be 44 24 2e       	movsbl 0x2e(%esp),%eax
 8048549:	89 54 24 08          	mov    %edx,0x8(%esp)
 804854d:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048551:	c7 04 24 02 00 00 00 	movl   $0x2,(%esp)
 8048558:	e8 97 fe ff ff       	call   80483f4 <varg>
 804855d:	0f bf 4c 24 2c       	movswl 0x2c(%esp),%ecx
 8048562:	0f be 54 24 2f       	movsbl 0x2f(%esp),%edx
 8048567:	0f be 44 24 2e       	movsbl 0x2e(%esp),%eax
 804856c:	89 4c 24 0c          	mov    %ecx,0xc(%esp)
 8048570:	89 54 24 08          	mov    %edx,0x8(%esp)
 8048574:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048578:	c7 04 24 03 00 00 00 	movl   $0x3,(%esp)
 804857f:	e8 70 fe ff ff       	call   80483f4 <varg>
 8048584:	0f bf 4c 24 2c       	movswl 0x2c(%esp),%ecx
 8048589:	0f be 54 24 2f       	movsbl 0x2f(%esp),%edx
 804858e:	0f be 44 24 2e       	movsbl 0x2e(%esp),%eax
 8048593:	8d 5c 24 28          	lea    0x28(%esp),%ebx
 8048597:	89 5c 24 10          	mov    %ebx,0x10(%esp)
 804859b:	89 4c 24 0c          	mov    %ecx,0xc(%esp)
 804859f:	89 54 24 08          	mov    %edx,0x8(%esp)
 80485a3:	89 44 24 04          	mov    %eax,0x4(%esp)
 80485a7:	c7 04 24 04 00 00 00 	movl   $0x4,(%esp)
 80485ae:	e8 41 fe ff ff       	call   80483f4 <varg>
 80485b3:	b8 00 00 00 00       	mov    $0x0,%eax
 80485b8:	83 c4 3c             	add    $0x3c,%esp
 80485bb:	5b                   	pop    %ebx
 80485bc:	89 ec                	mov    %ebp,%esp
 80485be:	5d                   	pop    %ebp
 80485bf:	c3                   	ret    

080485c0 <__libc_csu_fini>:
 80485c0:	55                   	push   %ebp
 80485c1:	89 e5                	mov    %esp,%ebp
 80485c3:	5d                   	pop    %ebp
 80485c4:	c3                   	ret    
 80485c5:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 80485c9:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

080485d0 <__libc_csu_init>:
 80485d0:	55                   	push   %ebp
 80485d1:	89 e5                	mov    %esp,%ebp
 80485d3:	57                   	push   %edi
 80485d4:	56                   	push   %esi
 80485d5:	53                   	push   %ebx
 80485d6:	e8 4f 00 00 00       	call   804862a <__i686.get_pc_thunk.bx>
 80485db:	81 c3 c5 11 00 00    	add    $0x11c5,%ebx
 80485e1:	83 ec 1c             	sub    $0x1c,%esp
 80485e4:	e8 d7 fc ff ff       	call   80482c0 <_init>
 80485e9:	8d bb 18 ff ff ff    	lea    -0xe8(%ebx),%edi
 80485ef:	8d 83 18 ff ff ff    	lea    -0xe8(%ebx),%eax
 80485f5:	29 c7                	sub    %eax,%edi
 80485f7:	c1 ff 02             	sar    $0x2,%edi
 80485fa:	85 ff                	test   %edi,%edi
 80485fc:	74 24                	je     8048622 <__libc_csu_init+0x52>
 80485fe:	31 f6                	xor    %esi,%esi
 8048600:	8b 45 10             	mov    0x10(%ebp),%eax
 8048603:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048607:	8b 45 0c             	mov    0xc(%ebp),%eax
 804860a:	89 44 24 04          	mov    %eax,0x4(%esp)
 804860e:	8b 45 08             	mov    0x8(%ebp),%eax
 8048611:	89 04 24             	mov    %eax,(%esp)
 8048614:	ff 94 b3 18 ff ff ff 	call   *-0xe8(%ebx,%esi,4)
 804861b:	83 c6 01             	add    $0x1,%esi
 804861e:	39 fe                	cmp    %edi,%esi
 8048620:	72 de                	jb     8048600 <__libc_csu_init+0x30>
 8048622:	83 c4 1c             	add    $0x1c,%esp
 8048625:	5b                   	pop    %ebx
 8048626:	5e                   	pop    %esi
 8048627:	5f                   	pop    %edi
 8048628:	5d                   	pop    %ebp
 8048629:	c3                   	ret    

0804862a <__i686.get_pc_thunk.bx>:
 804862a:	8b 1c 24             	mov    (%esp),%ebx
 804862d:	c3                   	ret    
 804862e:	90                   	nop
 804862f:	90                   	nop

08048630 <__do_global_ctors_aux>:
 8048630:	55                   	push   %ebp
 8048631:	89 e5                	mov    %esp,%ebp
 8048633:	53                   	push   %ebx
 8048634:	83 ec 04             	sub    $0x4,%esp
 8048637:	a1 b8 96 04 08       	mov    0x80496b8,%eax
 804863c:	83 f8 ff             	cmp    $0xffffffff,%eax
 804863f:	74 13                	je     8048654 <__do_global_ctors_aux+0x24>
 8048641:	bb b8 96 04 08       	mov    $0x80496b8,%ebx
 8048646:	66 90                	xchg   %ax,%ax
 8048648:	83 eb 04             	sub    $0x4,%ebx
 804864b:	ff d0                	call   *%eax
 804864d:	8b 03                	mov    (%ebx),%eax
 804864f:	83 f8 ff             	cmp    $0xffffffff,%eax
 8048652:	75 f4                	jne    8048648 <__do_global_ctors_aux+0x18>
 8048654:	83 c4 04             	add    $0x4,%esp
 8048657:	5b                   	pop    %ebx
 8048658:	5d                   	pop    %ebp
 8048659:	c3                   	ret    
 804865a:	90                   	nop
 804865b:	90                   	nop

Disassembly of section .fini:

0804865c <_fini>:
 804865c:	55                   	push   %ebp
 804865d:	89 e5                	mov    %esp,%ebp
 804865f:	53                   	push   %ebx
 8048660:	83 ec 04             	sub    $0x4,%esp
 8048663:	e8 00 00 00 00       	call   8048668 <_fini+0xc>
 8048668:	5b                   	pop    %ebx
 8048669:	81 c3 38 11 00 00    	add    $0x1138,%ebx
 804866f:	e8 fc fc ff ff       	call   8048370 <__do_global_dtors_aux>
 8048674:	59                   	pop    %ecx
 8048675:	5b                   	pop    %ebx
 8048676:	c9                   	leave  
 8048677:	c3                   	ret    
