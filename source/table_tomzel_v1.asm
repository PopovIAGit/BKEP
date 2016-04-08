; ������������������ ������� ������������� ������������
; ��������������� ������ �� � ������� ����������, ����
; � ���� ��������.

; �������������� ���������� ������������� � ����� ������� ������� CUB(3 �����������)
 ;---------------------------------------------------------------------------
 ;   ������		|���. ������| i��� | ���   |  ���	  |��������|   �����.   |
 ;  ��������	|  �������  |	   |������.|  ��	  |		   |  ��������  |
 ;---------------------------------------------------------------------------
 .def _drive1   ;   100 Nm,  i=52.4,  A-25, ���-�63�4,  0.37���, 1500 ��/���|
 .def _drive2	;   100 Nm,  i=52.4,  A-50, ���-�63�2,  0.37���, 3000 ��/���|
 .def _drive3	;   400 Nm,  i=73.7,  �-20, ���-�80S4,  0.55���, 1500 ��/���|
 .def _drive4	;   400 Nm,  i=73.7,  �-50, ���-�80�2,  1.5���,  3000 ��/���|
 .def _drive5	;   800 Nm,  i=73.7,  �-40, ���-�80�2,  1.5���,  3000 ��/���|
 .def _drive6	;  1000 Nm,  i=73.7,  �-20, ���-�80�4,  1.5���,  1500 ��/���|
 .def _drive7	;  4000 Nm,  i=167.2, �-9,  ���-�100S4, 2.5���,  1500 ��/���|
 .def _drive8	;  4000 Nm,  i=167.2, �-18, ���-�100S2, 4.0���,  3000 ��/���|
 .def _drive9	; 10000 Nm,  i=252.2, �-6,  ���-�100L4, 4.0���,  1500 ��/���|
 .def _drive10	; 10000 Nm,  i=270.2, �-12, ���-�100L2, 5.5���,  3000 ��/���|
 .def _drive11	; 10000 Nm,  i=174.2, �-10, �����-350,  11.0���, 1500 ��/���| ���2					|
 .def _drive12	; 15000 Nm	 i=144,   �-10, �����-350,  15.0���, 1500 ��/���|												|
 .def _drive13	; 20000 Nm,  i=144,   F-40, �����-350,  18.5���, 1500 ��/���|
 .def _drive14	;   400 Nm,  i=73.7,  �-40, ���-�80L2,  1.1���,  3000 ��/���|
 .def _drive15	;	 ?														|
 .def _drive16	;	 ?														|
 .def _drive17	;	 ?														|
 .def _drive18	;	 ?														|
 .def _drive19	;	 ?														|
 .def _drive20	;	 ?														|
;----------------------------------------------------------------------------

_drive1			;100 Nm,  i=52.4,  A-25, ���-�63�4,  0.37���, 1500 ��/���
 .word 1		;1.  ������ ��� 0_0
 .word 75		;2.  ������ ��� 0_1
 .word 105		;3.  ������ ��� 0_2
 .word 130		;4.  ������ ��� 0_3
 .word 130		;5.  ������ ��� 0_4
 .word 5		;6.  ������ ��� 1_0
 .word 50		;7.  ������ ��� 1_1
 .word 90		;8.  ������ ��� 1_2
 .word 176		;9.  ������ ��� 1_3
 .word 175		;10. ������ ��� 1_4
 .word 10		;11. ������ ��� 2_0
 .word 80		;12. ������ ��� 2_1
 .word 140		;13. ������ ��� 2_2
 .word 180		;14. ������ ��� 2_3
 .word 200		;15. ������ ��� 2_4
 .word 15		;16. ������ ��� 3_0
 .word 100		;17. ������ ��� 3_1
 .word 175		;18. ������ ��� 3_2
 .word 240		;19. ������ ��� 3_3
 .word 280		;20. ������ ��� 3_4
 .word 70		;21. ������ ���� 0_0
 .word 50		;22. ������ ���� 0_1
 .word 40		;23. ������ ���� 0_2
 .word 10		;24. ������ ���� 0_3
 .word 1		;25. ������ ���� 0_4
 .word 70		;26. ������ ���� 1_0
 .word 50		;27. ������ ���� 1_1
 .word 40		;28. ������ ���� 1_2
 .word 20		;29. ������ ���� 1_3
 .word 10		;30. ������ ���� 1_4
 .word 115		;31. ������ ���� 2_0
 .word 70		;32. ������ ���� 2_1
 .word 50		;33. ������ ���� 2_2
 .word 30		;34. ������ ���� 2_3
 .word 15		;35. ������ ���� 2_4
 .word 115		;36. ������ ���� 3_0
 .word 70		;37. ������ ���� 3_1
 .word 50		;38. ������ ���� 3_2
 .word 30		;39. ������ ���� 3_3
 .word 15		;40. ������ ���� 3_4
 .word 90		;41. ���� ���� 0_0 
 .word 86		;42. ���� ���� 0_1 
 .word 77		;43. ���� ���� 0_2
 .word 67		;44. ���� ���� 0_3
 .word 58		;45. ���� ���� 0_4
 .word 95		;46. ���� ���� 1_0
 .word 92		;47. ���� ���� 1_1
 .word 86		;48. ���� ���� 1_2
 .word 76		;49. ���� ���� 1_3
 .word 68		;50. ���� ���� 1_4
 .word 98		;51. ���� ���� 2_0
 .word 96		;52. ���� ���� 2_1
 .word 90		;53. ���� ���� 2_2
 .word 84		;54. ���� ���� 2_3
 .word 77		;55. ���� ���� 2_4
 .word 101		;56. ���� ���� 3_0
 .word 99		;57. ���� ���� 3_1
 .word 92		;58. ���� ���� 3_2
 .word 86		;59. ���� ���� 3_3
 .word 79		;60. ���� ���� 3_4

_drive2 	;   100 Nm,  i=52.4,  A-50, ���-�63�2,  0.37���, 3000 ��/���|
 .word 13		;1.  ������ ��� 0_0
 .word 40		;2.  ������ ��� 0_1
 .word 60		;3.  ������ ��� 0_2
 .word 80		;4.  ������ ��� 0_3
 .word 100		;5.  ������ ��� 0_4
 .word 10		;6.  ������ ��� 1_0
 .word 50		;7.  ������ ��� 1_1
 .word 80		;8.  ������ ��� 1_2
 .word 90		;9.  ������ ��� 1_3
 .word 100		;10. ������ ��� 1_4
 .word 1		;11. ������ ��� 2_0
 .word 60		;12. ������ ��� 2_1
 .word 85		;13. ������ ��� 2_2
 .word 120		;14. ������ ��� 2_3
 .word 140		;15. ������ ��� 2_4
 .word 1		;16. ������ ��� 3_0
 .word 1		;17. ������ ��� 3_1
 .word 90		;18. ������ ��� 3_2
 .word 130		;19. ������ ��� 3_3
 .word 150		;20. ������ ��� 3_4
 .word 50		;21. ������ ���� 0_0
 .word 20		;22. ������ ���� 0_1
 .word 4		;23. ������ ���� 0_2
 .word 1		;24. ������ ���� 0_3
 .word 1		;25. ������ ���� 0_4
 .word 65		;26. ������ ���� 1_0
 .word 35		;27. ������ ���� 1_1
 .word 16		;28. ������ ���� 1_2
 .word 3		;29. ������ ���� 1_3
 .word 1		;30. ������ ���� 1_4
 .word 87		;31. ������ ���� 2_0
 .word 52		;32. ������ ���� 2_1
 .word 35		;33. ������ ���� 2_2
 .word 14		;34. ������ ���� 2_3
 .word 1		;35. ������ ���� 2_4
 .word 120		;36. ������ ���� 3_0
 .word 90		;37. ������ ���� 3_1
 .word 60		;38. ������ ���� 3_2
 .word 25		;39. ������ ���� 3_3
 .word 1		;40. ������ ���� 3_4
 .word 80		;41. ���� ���� 0_0 
 .word 74		;42. ���� ���� 0_1 
 .word 63		;43. ���� ���� 0_2
 .word 29		;44. ���� ���� 0_3
 .word 2		;45. ���� ���� 0_4
 .word 87		;46. ���� ���� 1_0
 .word 81		;47. ���� ���� 1_1
 .word 75		;48. ���� ���� 1_2
 .word 55		;49. ���� ���� 1_3
 .word 31		;50. ���� ���� 1_4
 .word 92		;51. ���� ���� 2_0
 .word 86		;52. ���� ���� 2_1
 .word 77		;53. ���� ���� 2_2
 .word 68		;54. ���� ���� 2_3
 .word 47		;55. ���� ���� 2_4
 .word 94		;56. ���� ���� 3_0
 .word 89		;57. ���� ���� 3_1
 .word 82		;58. ���� ���� 3_2
 .word 72		;59. ���� ���� 3_3
 .word 60		;60. ���� ���� 3_4 

_drive3	; 400 Nm,  i=73.7,  �-20, ���-�80S4,  0.55���, 1500 ��/���|04.12.12  ��� 1.8�
 .word 5		;1.  ������ ��� 0_0
 .word 45		;2.  ������ ��� 0_1
 .word 66		;3.  ������ ��� 0_2
 .word 100		;4.  ������ ��� 0_3
 .word 120		;5.  ������ ��� 0_4
 .word 1		;6.  ������ ��� 1_0
 .word 50		;7.  ������ ��� 1_1
 .word 82		;8.  ������ ��� 1_2
 .word 110		;9.  ������ ��� 1_3
 .word 125		;10. ������ ��� 1_4
 .word 1		;11. ������ ��� 2_0
 .word 42		;12. ������ ��� 2_1
 .word 100		;13. ������ ��� 2_2
 .word 130		;14. ������ ��� 2_3
 .word 150		;15. ������ ��� 2_4
 .word 70		;16. ������ ��� 3_0
 .word 80		;17. ������ ��� 3_1
 .word 87		;18. ������ ��� 3_2
 .word 145		;19. ������ ��� 3_3
 .word 160		;20. ������ ��� 3_4
 .word 40		;21. ������ ���� 0_0
 .word 37		;22. ������ ���� 0_1
 .word 10		;23. ������ ���� 0_2
 .word 5		;24. ������ ���� 0_3
 .word 1		;25. ������ ���� 0_4
 .word 90		;26. ������ ���� 1_0
 .word 55		;27. ������ ���� 1_1
 .word 30		;28. ������ ���� 1_2
 .word 15		;29. ������ ���� 1_3
 .word 1		;30. ������ ���� 1_4
 .word 150		;31. ������ ���� 2_0
 .word 87		;32. ������ ���� 2_1
 .word 50		;33. ������ ���� 2_2
 .word 30		;34. ������ ���� 2_3
 .word 5		;35. ������ ���� 2_4
 .word 180		;36. ������ ���� 3_0
 .word 137		;37. ������ ���� 3_1
 .word 80		;38. ������ ���� 3_2
 .word 36		;39. ������ ���� 3_3
 .word 1		;40. ������ ���� 3_4
 .word 80		;41. ���� ���� 0_0 
 .word 75		;42. ���� ���� 0_1 
 .word 73		;43. ���� ���� 0_2
 .word 57		;44. ���� ���� 0_3
 .word 92		;45. ���� ���� 0_4
 .word 92		;46. ���� ���� 1_0
 .word 87		;47. ���� ���� 1_1
 .word 80		;48. ���� ���� 1_2
 .word 67		;49. ���� ���� 1_3
 .word 54		;50. ���� ���� 1_4
 .word 96		;51. ���� ���� 2_0
 .word 93		;52. ���� ���� 2_1
 .word 85		;53. ���� ���� 2_2
 .word 72		;54. ���� ���� 2_3
 .word 63		;55. ���� ���� 2_4
 .word 98		;56. ���� ���� 3_0
 .word 94		;57. ���� ���� 3_1
 .word 90		;58. ���� ���� 3_2
 .word 81		;59. ���� ���� 3_3
 .word 70		;60. ���� ���� 3_4  


_drive4		;   400 Nm,  i=73.7,  �-50, ���-�80�2,  1.5���,  3000 ��/���|
 .word 5		;1.  ������ ��� 0_0
 .word 45		;2.  ������ ��� 0_1
 .word 66		;3.  ������ ��� 0_2
 .word 100		;4.  ������ ��� 0_3
 .word 120		;5.  ������ ��� 0_4
 .word 1		;6.  ������ ��� 1_0
 .word 45		;7.  ������ ��� 1_1
 .word 75		;8.  ������ ��� 1_2
 .word 100		;9.  ������ ��� 1_3
 .word 110		;10. ������ ��� 1_4
 .word 15		;11. ������ ��� 2_0
 .word 62		;12. ������ ��� 2_1
 .word 87		;13. ������ ��� 2_2
 .word 110		;14. ������ ��� 2_3
 .word 125		;15. ������ ��� 2_4
 .word 70		;16. ������ ��� 3_0
 .word 80		;17. ������ ��� 3_1
 .word 87		;18. ������ ��� 3_2
 .word 125		;19. ������ ��� 3_3
 .word 135		;20. ������ ��� 3_4
 .word 40		;21. ������ ���� 0_0
 .word 37		;22. ������ ���� 0_1
 .word 10		;23. ������ ���� 0_2
 .word 5		;24. ������ ���� 0_3
 .word 1		;25. ������ ���� 0_4
 .word 65		;26. ������ ���� 1_0
 .word 45		;27. ������ ���� 1_1
 .word 20		;28. ������ ���� 1_2
 .word 11		;29. ������ ���� 1_3
 .word 1		;30. ������ ���� 1_4
 .word 85		;31. ������ ���� 2_0
 .word 43		;32. ������ ���� 2_1
 .word 25		;33. ������ ���� 2_2
 .word 12		;34. ������ ���� 2_3
 .word 5		;35. ������ ���� 2_4
 .word 95		;36. ������ ���� 3_0
 .word 65		;37. ������ ���� 3_1
 .word 40		;38. ������ ���� 3_2
 .word 24		;39. ������ ���� 3_3
 .word 1		;40. ������ ���� 3_4
 .word 80		;41. ���� ���� 0_0 
 .word 75		;42. ���� ���� 0_1 
 .word 73		;43. ���� ���� 0_2
 .word 57		;44. ���� ���� 0_3
 .word 92		;45. ���� ���� 0_4
 .word 92		;46. ���� ���� 1_0
 .word 87		;47. ���� ���� 1_1
 .word 80		;48. ���� ���� 1_2
 .word 67		;49. ���� ���� 1_3
 .word 54		;50. ���� ���� 1_4
 .word 95		;51. ���� ���� 2_0
 .word 92		;52. ���� ���� 2_1
 .word 83		;53. ���� ���� 2_2
 .word 75		;54. ���� ���� 2_3
 .word 65		;55. ���� ���� 2_4
 .word 98		;56. ���� ���� 3_0
 .word 94		;57. ���� ���� 3_1
 .word 90		;58. ���� ���� 3_2
 .word 81		;59. ���� ���� 3_3
 .word 70		;60. ���� ���� 3_4   
 
_drive5	;   800 Nm,  i=73.7,  �-40, ���-�80�2,  1.5���,  3000 ��/���|  �� ������ - ������� ������ 04.12.12
 .word 12		;1.  ������ ��� 0_0
 .word 30		;2.  ������ ��� 0_1
 .word 60		;3.  ������ ��� 0_2
 .word 67		;4.  ������ ��� 0_3
 .word 80		;5.  ������ ��� 0_4
 .word 10		;6.  ������ ��� 1_0
 .word 55		;7.  ������ ��� 1_1
 .word 76		;8.  ������ ��� 1_2
 .word 95		;9.  ������ ��� 1_3
 .word 110		;10. ������ ��� 1_4
 .word 1		;11. ������ ��� 2_0
 .word 58		;12. ������ ��� 2_1
 .word 92		;13. ������ ��� 2_2
 .word 110		;14. ������ ��� 2_3
 .word 125		;15. ������ ��� 2_4
 .word 1		;16. ������ ��� 3_0
 .word 1		;17. ������ ��� 3_1
 .word 96		;18. ������ ��� 3_2
 .word 125		;19. ������ ��� 3_3
 .word 160		;20. ������ ��� 3_4
 .word 50		;21. ������ ���� 0_0
 .word 40		;22. ������ ���� 0_1
 .word 11		;23. ������ ���� 0_2
 .word 1		;24. ������ ���� 0_3
 .word 1		;25. ������ ���� 0_4
 .word 70		;26. ������ ���� 1_0
 .word 66		;27. ������ ���� 1_1
 .word 28		;28. ������ ���� 1_2
 .word 14		;29. ������ ���� 1_3
 .word 9		;30. ������ ���� 1_4
 .word 100		;31. ������ ���� 2_0
 .word 85		;32. ������ ���� 2_1
 .word 45		;33. ������ ���� 2_2
 .word 21		;34. ������ ���� 2_3
 .word 12		;35. ������ ���� 2_4
 .word 150		;36. ������ ���� 3_0
 .word 130		;37. ������ ���� 3_1
 .word 81		;38. ������ ���� 3_2
 .word 38		;39. ������ ���� 3_3
 .word 10		;40. ������ ���� 3_4
 .word 68		;41. ���� ���� 0_0 
 .word 50		;42. ���� ���� 0_1 
 .word 35		;43. ���� ���� 0_2
 .word 20		;44. ���� ���� 0_3
 .word 10		;45. ���� ���� 0_4
 .word 78		;46. ���� ���� 1_0
 .word 63		;47. ���� ���� 1_1
 .word 52		;48. ���� ���� 1_2
 .word 25		;49. ���� ���� 1_3
 .word 10		;50. ���� ���� 1_4
 .word 87		;51. ���� ���� 2_0
 .word 78		;52. ���� ���� 2_1
 .word 70		;53. ���� ���� 2_2
 .word 47		;54. ���� ���� 2_3
 .word 29		;55. ���� ���� 2_4
 .word 88		;56. ���� ���� 3_0
 .word 80		;57. ���� ���� 3_1 
 .word 71		;58. ���� ���� 3_2
 .word 52		;59. ���� ���� 3_3
 .word 5		;60. ���� ���� 3_4 	

_drive6	;  1000 Nm,  i=73.7,  �-20, ���-�80�4,  1.5���,  1500 ��/���|
 .word 12		;1.  ������ ��� 0_0
 .word 30		;2.  ������ ��� 0_1
 .word 60		;3.  ������ ��� 0_2
 .word 67		;4.  ������ ��� 0_3
 .word 80		;5.  ������ ��� 0_4
 .word 10		;6.  ������ ��� 1_0
 .word 55		;7.  ������ ��� 1_1
 .word 76		;8.  ������ ��� 1_2
 .word 95		;9.  ������ ��� 1_3
 .word 110		;10. ������ ��� 1_4
 .word 1		;11. ������ ��� 2_0
 .word 58		;12. ������ ��� 2_1
 .word 92		;13. ������ ��� 2_2
 .word 110		;14. ������ ��� 2_3
 .word 125		;15. ������ ��� 2_4
 .word 1		;16. ������ ��� 3_0
 .word 1		;17. ������ ��� 3_1
 .word 96		;18. ������ ��� 3_2
 .word 125		;19. ������ ��� 3_3
 .word 160		;20. ������ ��� 3_4
 .word 50		;21. ������ ���� 0_0
 .word 40		;22. ������ ���� 0_1
 .word 11		;23. ������ ���� 0_2
 .word 1		;24. ������ ���� 0_3
 .word 1		;25. ������ ���� 0_4
 .word 70		;26. ������ ���� 1_0
 .word 66		;27. ������ ���� 1_1
 .word 28		;28. ������ ���� 1_2
 .word 14		;29. ������ ���� 1_3
 .word 9		;30. ������ ���� 1_4
 .word 100		;31. ������ ���� 2_0
 .word 85		;32. ������ ���� 2_1
 .word 45		;33. ������ ���� 2_2
 .word 21		;34. ������ ���� 2_3
 .word 12		;35. ������ ���� 2_4
 .word 150		;36. ������ ���� 3_0
 .word 130		;37. ������ ���� 3_1
 .word 81		;38. ������ ���� 3_2
 .word 38		;39. ������ ���� 3_3
 .word 10		;40. ������ ���� 3_4
 .word 80		;41. ���� ���� 0_0 
 .word 75		;42. ���� ���� 0_1 
 .word 64		;43. ���� ���� 0_2
 .word 43		;44. ���� ���� 0_3
 .word 35		;45. ���� ���� 0_4
 .word 87		;46. ���� ���� 1_0
 .word 82		;47. ���� ���� 1_1
 .word 76		;48. ���� ���� 1_2
 .word 61		;49. ���� ���� 1_3
 .word 45		;50. ���� ���� 1_4
 .word 94		;51. ���� ���� 2_0
 .word 86		;52. ���� ���� 2_1
 .word 78		;53. ���� ���� 2_2
 .word 67		;54. ���� ���� 2_3
 .word 60		;55. ���� ���� 2_4
 .word 94		;56. ���� ���� 3_0
 .word 92		;57. ���� ���� 3_1 
 .word 87		;58. ���� ���� 3_2
 .word 78		;59. ���� ���� 3_3
 .word 69		;60. ���� ���� 3_4 	


_drive7			;  4000 Nm,  i=167.2, �-9,  ���-�100S4, 2.5���,  1500 ��/��� 5,5A
 .word 8		;1.  ������ ��� 0_0
 .word 35		;2.  ������ ��� 0_1
 .word 52		;3.  ������ ��� 0_2
 .word 68		;4.  ������ ��� 0_3
 .word 80		;5.  ������ ��� 0_4
 .word 1		;6.  ������ ��� 1_0
 .word 1		;7.  ������ ��� 1_1
 .word 38		;8.  ������ ��� 1_2
 .word 70		;9.  ������ ��� 1_3
 .word 102		;10. ������ ��� 1_4
 .word 1		;11. ������ ��� 2_0
 .word 1		;12. ������ ��� 2_1
 .word 60		;13. ������ ��� 2_2
 .word 80		;14. ������ ��� 2_3
 .word 102		;15. ������ ��� 2_4
 .word 1		;16. ������ ��� 3_0
 .word 1		;17. ������ ��� 3_1
 .word 70		;18. ������ ��� 3_2
 .word 90		;19. ������ ��� 3_3
 .word 110		;20. ������ ��� 3_4
 .word 61		;21. ������ ���� 0_0
 .word 61		;22. ������ ���� 0_1
 .word 14		;23. ������ ���� 0_2
 .word 7		;24. ������ ���� 0_3
 .word 1		;25. ������ ���� 0_4
 .word 86		;26. ������ ���� 1_0
 .word 43		;27. ������ ���� 1_1
 .word 19		;28. ������ ���� 1_2
 .word 14		;29. ������ ���� 1_3
 .word 3		;30. ������ ���� 1_4
 .word 111		;31. ������ ���� 2_0
 .word 80		;32. ������ ���� 2_1
 .word 42		;33. ������ ���� 2_2
 .word 20		;34. ������ ���� 2_3
 .word 1		;35. ������ ���� 2_4
 .word 115		;36. ������ ���� 3_0
 .word 85		;37. ������ ���� 3_1
 .word 50		;38. ������ ���� 3_2
 .word 25		;39. ������ ���� 3_3
 .word 1		;40. ������ ���� 3_4
 .word 82		;41. ���� ���� 0_0 
 .word 77		;42. ���� ���� 0_1 
 .word 64		;43. ���� ���� 0_2
 .word 57		;44. ���� ���� 0_3
 .word 27		;45. ���� ���� 0_4
 .word 88		;46. ���� ���� 1_0
 .word 85		;47. ���� ���� 1_1
 .word 78		;48. ���� ���� 1_2
 .word 66		;49. ���� ���� 1_3
 .word 55		;50. ���� ���� 1_4
 .word 92		;51. ���� ���� 2_0
 .word 89		;52. ���� ���� 2_1
 .word 82		;53. ���� ���� 2_2
 .word 72		;54. ���� ���� 2_3
 .word 65		;55. ���� ���� 2_4
 .word 94		;56. ���� ���� 3_0
 .word 90		;57. ���� ���� 3_1
 .word 86		;58. ���� ���� 3_2
 .word 79		;59. ���� ���� 3_3
 .word 71		;60. ���� ���� 3_4 

_drive8		    ; 4000 Nm,  i=167.2, �-18,  ���-�100S4, 4���,  3000 ��/��� 11A 
 .word 27		;1.  ������ ��� 0_0
 .word 45		;2.  ������ ��� 0_1
 .word 65		;3.  ������ ��� 0_2
 .word 70		;4.  ������ ��� 0_3
 .word 70		;5.  ������ ��� 0_4
 .word 25		;6.  ������ ��� 1_0
 .word 54		;7.  ������ ��� 1_1
 .word 70		;8.  ������ ��� 1_2
 .word 84		;9.  ������ ��� 1_3
 .word 100		;10. ������ ��� 1_4
 .word 1		;11. ������ ��� 2_0
 .word 50		;12. ������ ��� 2_1
 .word 69		;13. ������ ��� 2_2
 .word 90		;14. ������ ��� 2_3
 .word 110		;15. ������ ��� 2_4
 .word 10		;16. ������ ��� 3_0
 .word 60		;17. ������ ��� 3_1
 .word 85		;18. ������ ��� 3_2
 .word 100		;19. ������ ��� 3_3
 .word 120		;20. ������ ��� 3_4
 .word 36		;21. ������ ���� 0_0
 .word 20		;22. ������ ���� 0_1
 .word 13		;23. ������ ���� 0_2
 .word 9		;24. ������ ���� 0_3
 .word 1		;25. ������ ���� 0_4
 .word 67		;26. ������ ���� 1_0
 .word 37		;27. ������ ���� 1_1
 .word 17		;28. ������ ���� 1_2
 .word 10		;29. ������ ���� 1_3
 .word 1		;30. ������ ���� 1_4
 .word 100		;31. ������ ���� 2_0
 .word 70		;32. ������ ���� 2_1
 .word 35		;33. ������ ���� 2_2
 .word 8		;34. ������ ���� 2_3
 .word 2		;35. ������ ���� 2_4
 .word 110		;36. ������ ���� 3_0
 .word 90		;37. ������ ���� 3_1
 .word 47		;38. ������ ���� 3_2
 .word 8		;39. ������ ���� 3_3
 .word 5		;40. ������ ���� 3_4
 .word 76		;41. ���� ���� 0_0 
 .word 70		;42. ���� ���� 0_1 
 .word 61		;43. ���� ���� 0_2
 .word 40		;44. ���� ���� 0_3
 .word 20		;45. ���� ���� 0_4
 .word 80		;46. ���� ���� 1_0
 .word 77		;47. ���� ���� 1_1
 .word 69		;48. ���� ���� 1_2
 .word 55		;49. ���� ���� 1_3
 .word 25		;50. ���� ���� 1_4
 .word 82		;51. ���� ���� 2_0
 .word 78		;52. ���� ���� 2_1
 .word 70		;53. ���� ���� 2_2
 .word 57		;54. ���� ���� 2_3
 .word 36		;55. ���� ���� 2_4
 .word 84		;56. ���� ���� 3_0
 .word 80		;57. ���� ���� 3_1
 .word 75		;58. ���� ���� 3_2
 .word 65		;59. ���� ���� 3_3
 .word 55		;60. ���� ���� 3_4 

_drive9		; 10000 Nm, ipp = 100%  i=252.2, �-6,  ���-�100L4, 4.0���,  1500 ��/���|
 .word 11		;1.  ������ ��� 0_0
 .word 32		;2.  ������ ��� 0_1
 .word 46		;3.  ������ ��� 0_2
 .word 58		;4.  ������ ��� 0_3
 .word 67		;5.  ������ ��� 0_4
 .word 1		;6.  ������ ��� 1_0
 .word 36		;7.  ������ ��� 1_1
 .word 58		;8.  ������ ��� 1_2
 .word 75		;9.  ������ ��� 1_3
 .word 84		;10. ������ ��� 1_4
 .word 1		;11. ������ ��� 2_0
 .word 29		;12. ������ ��� 2_1
 .word 70		;13. ������ ��� 2_2
 .word 86		;14. ������ ��� 2_3
 .word 110		;15. ������ ��� 2_4
 .word 1		;16. ������ ��� 3_0
 .word 1		;17. ������ ��� 3_1
 .word 77		;18. ������ ��� 3_2
 .word 90		;19. ������ ��� 3_3
 .word 130		;20. ������ ��� 3_4
 .word 30		;21. ������ ���� 0_0
 .word 23		;22. ������ ���� 0_1
 .word 12		;23. ������ ���� 0_2
 .word 6		;24. ������ ���� 0_3
 .word 1		;25. ������ ���� 0_4
 .word 45		;26. ������ ���� 1_0
 .word 40		;27. ������ ���� 1_1
 .word 23		;28. ������ ���� 1_2
 .word 11		;29. ������ ���� 1_3
 .word 3		;30. ������ ���� 1_4
 .word 70		;31. ������ ���� 2_0
 .word 67		;32. ������ ���� 2_1
 .word 38		;33. ������ ���� 2_2
 .word 24		;34. ������ ���� 2_3
 .word 6		;35. ������ ���� 2_4
 .word 120		;36. ������ ���� 3_0
 .word 110		;37. ������ ���� 3_1
 .word 70		;38. ������ ���� 3_2
 .word 37		;39. ������ ���� 3_3
 .word 8		;40. ������ ���� 3_4
 .word 82		;41. ���� ���� 0_0 
 .word 75		;42. ���� ���� 0_1 
 .word 65		;43. ���� ���� 0_2
 .word 41		;44. ���� ���� 0_3
 .word 40		;45. ���� ���� 0_4
 .word 88		;46. ���� ���� 1_0
 .word 82		;47. ���� ���� 1_1
 .word 74		;48. ���� ���� 1_2
 .word 60		;49. ���� ���� 1_3
 .word 43		;50. ���� ���� 1_4
 .word 91		;51. ���� ���� 2_0
 .word 86		;52. ���� ���� 2_1
 .word 76		;53. ���� ���� 2_2
 .word 68		;54. ���� ���� 2_3
 .word 57		;55. ���� ���� 2_4
 .word 94		;56. ���� ���� 3_0
 .word 90		;57. ���� ���� 3_1 91
 .word 85		;58. ���� ���� 3_2
 .word 75		;59. ���� ���� 3_3
 .word 65		;60. ���� ���� 3_4	 

_drive10 		; 10000 Nm,  i=270.2, �-12, ���-�100L2, 5.5���,  3000 ��/���|
 .word 8		;1.  ������ ��� 0_0
 .word 25		;2.  ������ ��� 0_1
 .word 35		;3.  ������ ��� 0_2
 .word 40		;4.  ������ ��� 0_3
 .word 50		;5.  ������ ��� 0_4
 .word 8		;6.  ������ ��� 1_0
 .word 28		;7.  ������ ��� 1_1
 .word 55		;8.  ������ ��� 1_2
 .word 65		;9.  ������ ��� 1_3
 .word 85		;10. ������ ��� 1_4
 .word 1		;11. ������ ��� 2_0
 .word 37		;12. ������ ��� 2_1
 .word 60		;13. ������ ��� 2_2
 .word 80		;14. ������ ��� 2_3
 .word 115		;15. ������ ��� 2_4
 .word 1		;16. ������ ��� 3_0
 .word 45		;17. ������ ��� 3_1
 .word 74		;18. ������ ��� 3_2
 .word 94		;19. ������ ��� 3_3
 .word 120		;20. ������ ��� 3_4
 .word 25		;21. ������ ���� 0_0
 .word 15		;22. ������ ���� 0_1
 .word 4		;23. ������ ���� 0_2
 .word 1		;24. ������ ���� 0_3
 .word 0		;25. ������ ���� 0_4
 .word 40		;26. ������ ���� 1_0
 .word 20		;27. ������ ���� 1_1
 .word 10		;28. ������ ���� 1_2
 .word 3		;29. ������ ���� 1_3
 .word 0		;30. ������ ���� 1_4
 .word 62		;31. ������ ���� 2_0
 .word 32		;32. ������ ���� 2_1
 .word 20		;33. ������ ���� 2_2
 .word 1		;34. ������ ���� 2_3
 .word 1		;35. ������ ���� 2_4
 .word 70		;36. ������ ���� 3_0
 .word 43		;37. ������ ���� 3_1
 .word 25		;38. ������ ���� 3_2
 .word 2		;39. ������ ���� 3_3
 .word 1		;40. ������ ���� 3_4
 .word 83		;41. ���� ���� 0_0 
 .word 73		;42. ���� ���� 0_1 
 .word 60		;43. ���� ���� 0_2
 .word 50		;44. ���� ���� 0_3
 .word 50		;45. ���� ���� 0_4
 .word 89		;46. ���� ���� 1_0
 .word 83		;47. ���� ���� 1_1
 .word 72		;48. ���� ���� 1_2
 .word 50		;49. ���� ���� 1_3
 .word 30		;50. ���� ���� 1_4
 .word 90		;51. ���� ���� 2_0
 .word 86		;52. ���� ���� 2_1
 .word 79		;53. ���� ���� 2_2
 .word 68		;54. ���� ���� 2_3
 .word 48		;55. ���� ���� 2_4
 .word 98		;56. ���� ���� 3_0
 .word 91		;57. ���� ���� 3_1
 .word 82		;58. ���� ���� 3_2
 .word 70		;59. ���� ���� 3_3
 .word 52		;60. ���� ���� 3_4	

_drive11 		; 10000 Nm,  i=174.2, �-10, �����-350,  11.0���, 1500 ��/���| ���2 �� �����!!!
 .word 8		;1.  ������ ��� 0_0
 .word 25		;2.  ������ ��� 0_1
 .word 35		;3.  ������ ��� 0_2
 .word 40		;4.  ������ ��� 0_3
 .word 50		;5.  ������ ��� 0_4
 .word 8		;6.  ������ ��� 1_0
 .word 28		;7.  ������ ��� 1_1
 .word 55		;8.  ������ ��� 1_2
 .word 65		;9.  ������ ��� 1_3
 .word 85		;10. ������ ��� 1_4
 .word 16		;11. ������ ��� 2_0
 .word 48		;12. ������ ��� 2_1
 .word 68		;13. ������ ��� 2_2
 .word 87		;14. ������ ��� 2_3
 .word 125		;15. ������ ��� 2_4
 .word 0		;16. ������ ��� 3_0
 .word 40		;17. ������ ��� 3_1
 .word 74		;18. ������ ��� 3_2
 .word 94		;19. ������ ��� 3_3
 .word 130		;20. ������ ��� 3_4
 .word 25		;21. ������ ���� 0_0
 .word 15		;22. ������ ���� 0_1
 .word 4		;23. ������ ���� 0_2
 .word 1		;24. ������ ���� 0_3
 .word 0		;25. ������ ���� 0_4
 .word 40		;26. ������ ���� 1_0
 .word 20		;27. ������ ���� 1_1
 .word 10		;28. ������ ���� 1_2
 .word 3		;29. ������ ���� 1_3
 .word 0		;30. ������ ���� 1_4
 .word 55		;31. ������ ���� 2_0
 .word 30		;32. ������ ���� 2_1
 .word 15		;33. ������ ���� 2_2
 .word 8		;34. ������ ���� 2_3
 .word 0		;35. ������ ���� 2_4
 .word 70		;36. ������ ���� 3_0
 .word 43		;37. ������ ���� 3_1
 .word 26		;38. ������ ���� 3_2
 .word 13		;39. ������ ���� 3_3
 .word 2		;40. ������ ���� 3_4
 .word 83		;41. ���� ���� 0_0
 .word 73		;42. ���� ���� 0_1
 .word 60		;43. ���� ���� 0_2
 .word 50		;44. ���� ���� 0_3
 .word 50		;45. ���� ���� 0_4
 .word 89		;46. ���� ���� 1_0
 .word 83		;47. ���� ���� 1_1
 .word 72		;48. ���� ���� 1_2
 .word 50		;49. ���� ���� 1_3
 .word 30		;50. ���� ���� 1_4
 .word 90		;51. ���� ���� 2_0
 .word 86		;52. ���� ���� 2_1
 .word 79		;53. ���� ���� 2_2
 .word 68		;54. ���� ���� 2_3
 .word 48		;55. ���� ���� 2_4
 .word 98		;56. ���� ���� 3_0
 .word 91		;57. ���� ���� 3_1
 .word 82		;58. ���� ���� 3_2
 .word 70		;59. ���� ���� 3_3
 .word 52		;60. ���� ���� 3_4

_drive12 		; 15000 Nm	 i=144,   �-10, �����-350,  15.0���, 1500 ��/���| ���2 ��� 160 190 � 250�!!!
 .word 13		;1.  ������ ��� 0_0
 .word 53		;2.  ������ ��� 0_1
 .word 80		;3.  ������ ��� 0_2
 .word 100		;4.  ������ ��� 0_3
 .word 120		;5.  ������ ��� 0_4
 .word 13		;6.  ������ ��� 1_0
 .word 53		;7.  ������ ��� 1_1
 .word 80		;8.  ������ ��� 1_2
 .word 100		;9.  ������ ��� 1_3
 .word 120		;10. ������ ��� 1_4
 .word 13		;11. ������ ��� 2_0
 .word 53		;12. ������ ��� 2_1
 .word 80		;13. ������ ��� 2_2
 .word 100		;14. ������ ��� 2_3
 .word 120		;15. ������ ��� 2_4
 .word 13		;16. ������ ��� 3_0
 .word 53		;17. ������ ��� 3_1
 .word 80		;18. ������ ��� 3_2
 .word 100		;19. ������ ��� 3_3
 .word 120		;20. ������ ��� 3_4
 .word 86		;21. ������ ���� 0_0
 .word 43		;22. ������ ���� 0_1
 .word 23		;23. ������ ���� 0_2
 .word 1		;24. ������ ���� 0_3
 .word 1		;25. ������ ���� 0_4
 .word 86		;26. ������ ���� 1_0
 .word 43		;27. ������ ���� 1_1
 .word 23		;28. ������ ���� 1_2
 .word 3		;29. ������ ���� 1_3
 .word 1		;30. ������ ���� 1_4
 .word 86		;31. ������ ���� 2_0
 .word 43		;32. ������ ���� 2_1
 .word 23		;33. ������ ���� 2_2
 .word 4		;34. ������ ���� 2_3
 .word 1		;35. ������ ���� 2_4
 .word 86		;36. ������ ���� 3_0
 .word 43		;37. ������ ���� 3_1
 .word 26		;38. ������ ���� 3_2
 .word 13		;39. ������ ���� 3_3
 .word 2		;40. ������ ���� 3_4
 .word 97		;41. ���� ���� 0_0
 .word 91		;42. ���� ���� 0_1
 .word 80		;43. ���� ���� 0_2
 .word 75		;44. ���� ���� 0_3
 .word 71		;45. ���� ���� 0_4
 .word 97		;46. ���� ���� 1_0
 .word 91		;47. ���� ���� 1_1
 .word 80		;48. ���� ���� 1_2
 .word 75		;49. ���� ���� 1_3
 .word 71		;50. ���� ���� 1_4
 .word 97		;51. ���� ���� 2_0
 .word 91		;52. ���� ���� 2_1
 .word 80		;53. ���� ���� 2_2
 .word 75		;54. ���� ���� 2_3
 .word 71		;55. ���� ���� 2_4
 .word 98		;56. ���� ���� 3_0
 .word 91		;57. ���� ���� 3_1
 .word 82		;58. ���� ���� 3_2
 .word 75		;59. ���� ���� 3_3
 .word 71		;60. ���� ���� 3_4

_drive13 		; 20000 Nm,  i=144,   F-40, �����-350,  18.5���, 1500 ��/���| ���2 �� �����!!!
 .word 1		;1.  ������ ��� 0_0 +
 .word 27		;2.  ������ ��� 0_1 +
 .word 40		;3.  ������ ��� 0_2 +
 .word 70		;4.  ������ ��� 0_3 +
 .word 80		;5.  ������ ��� 0_4 +
 .word 1		;6.  ������ ��� 1_0 +
 .word 30		;7.  ������ ��� 1_1 +
 .word 55		;8.  ������ ��� 1_2 +
 .word 80		;9.  ������ ��� 1_3 +
 .word 105		;10. ������ ��� 1_4 +
 .word 1		;11. ������ ��� 2_0 +
 .word 60		;12. ������ ��� 2_1 +
 .word 85		;13. ������ ��� 2_2 +
 .word 110		;14. ������ ��� 2_3 +
 .word 130		;15. ������ ��� 2_4 +
 .word 5		;16. ������ ��� 3_0 +
 .word 60		;17. ������ ��� 3_1 +
 .word 85		;18. ������ ��� 3_2 +
 .word 110		;19. ������ ��� 3_3 +
 .word 130		;20. ������ ��� 3_4 +
 .word 55		;21. ������ ���� 0_0 +
 .word 30		;22. ������ ���� 0_1 +
 .word 10		;23. ������ ���� 0_2 +
 .word 3		;24. ������ ���� 0_3 +
 .word 0		;25. ������ ���� 0_4 +
 .word 80		;26. ������ ���� 1_0 +
 .word 40		;27. ������ ���� 1_1 +
 .word 15		;28. ������ ���� 1_2 +
 .word 1		;29. ������ ���� 1_3 +
 .word 1		;30. ������ ���� 1_4 +
 .word 105		;31. ������ ���� 2_0 +
 .word 60		;32. ������ ���� 2_1 +
 .word 30		;33. ������ ���� 2_2 +
 .word 15		;34. ������ ���� 2_3 +
 .word 1		;35. ������ ���� 2_4 +
 .word 110		;36. ������ ���� 3_0 +
 .word 60		;37. ������ ���� 3_1 +
 .word 30		;38. ������ ���� 3_2 +
 .word 15		;39. ������ ���� 3_3 +
 .word 3		;40. ������ ���� 3_4 +
 .word 87		;41. ���� ���� 0_0 +
 .word 83		;42. ���� ���� 0_1 +
 .word 76		;43. ���� ���� 0_2 +
 .word 65		;44. ���� ���� 0_3 +
 .word 60		;45. ���� ���� 0_4 +
 .word 93		;46. ���� ���� 1_0 +
 .word 89		;47. ���� ���� 1_1 +
 .word 82		;48. ���� ���� 1_2 +
 .word 71		;49. ���� ���� 1_3 +
 .word 66		;50. ���� ���� 1_4 +
 .word 96		;51. ���� ���� 2_0 +
 .word 92		;52. ���� ���� 2_1 +
 .word 85		;53. ���� ���� 2_2 +
 .word 74		;54. ���� ���� 2_3 +
 .word 69		;55. ���� ���� 2_4 +
 .word 99		;56. ���� ���� 3_0 +
 .word 96		;57. ���� ���� 3_1 +
 .word 89		;58. ���� ���� 3_2 +
 .word 83		;59. ���� ���� 3_3 +
 .word 78		;60. ���� ���� 3_4 +

_drive14	    ; ����� 50000
 .word 1		;1.  ������ ��� 0_0 +
 .word 27		;2.  ������ ��� 0_1 +
 .word 40		;3.  ������ ��� 0_2 +
 .word 60		;4.  ������ ��� 0_3 +
 .word 85		;5.  ������ ��� 0_4 +
 .word 6		;6.  ������ ��� 1_0 +
 .word 29		;7.  ������ ��� 1_1 +
 .word 52		;8.  ������ ��� 1_2 +
 .word 82		;9.  ������ ��� 1_3 +
 .word 105		;10. ������ ��� 1_4 +
 .word 12		;11. ������ ��� 2_0 +
 .word 38		;12. ������ ��� 2_1 +
 .word 60		;13. ������ ��� 2_2 +
 .word 87		;14. ������ ��� 2_3 +
 .word 117		;15. ������ ��� 2_4 +
 .word 17		;16. ������ ��� 3_0 +
 .word 59		;17. ������ ��� 3_1 +
 .word 77		;18. ������ ��� 3_2 +
 .word 98		;19. ������ ��� 3_3 +
 .word 125		;20. ������ ��� 3_4 +
 .word 70		;21. ������ ���� 0_0 +
 .word 30		;22. ������ ���� 0_1 +
 .word 10		;23. ������ ���� 0_2 +
 .word 3		;24. ������ ���� 0_3 +
 .word 1		;25. ������ ���� 0_4 +
 .word 90		;26. ������ ���� 1_0 +
 .word 52		;27. ������ ���� 1_1 +
 .word 22 		;28. ������ ���� 1_2 +
 .word 10		;29. ������ ���� 1_3 +
 .word 6		;30. ������ ���� 1_4 +
 .word 110		;31. ������ ���� 2_0 +
 .word 60		;32. ������ ���� 2_1 +
 .word 30		;33. ������ ���� 2_2 +
 .word 24		;34. ������ ���� 2_3 +
 .word 9		;35. ������ ���� 2_4 +
 .word 120		;36. ������ ���� 3_0 +
 .word 70		;37. ������ ���� 3_1 +
 .word 42		;38. ������ ���� 3_2 +
 .word 28		;39. ������ ���� 3_3 +
 .word 11		;40. ������ ���� 3_4 +
 .word 87		;41. ���� ���� 0_0 +
 .word 83		;42. ���� ���� 0_1 +
 .word 76		;43. ���� ���� 0_2 +
 .word 65		;44. ���� ���� 0_3 +
 .word 60		;45. ���� ���� 0_4 +
 .word 93		;46. ���� ���� 1_0 +
 .word 89		;47. ���� ���� 1_1 +
 .word 82		;48. ���� ���� 1_2 +
 .word 71		;49. ���� ���� 1_3 +
 .word 66		;50. ���� ���� 1_4 +
 .word 96		;51. ���� ���� 2_0 +
 .word 92		;52. ���� ���� 2_1 +
 .word 85		;53. ���� ���� 2_2 +
 .word 74		;54. ���� ���� 2_3 +
 .word 69		;55. ���� ���� 2_4 +
 .word 99		;56. ���� ���� 3_0 +
 .word 96		;57. ���� ���� 3_1 +
 .word 89		;58. ���� ���� 3_2 +
 .word 83		;59. ���� ���� 3_3 +
 .word 78		;60. ���� ���� 3_4 +

_drive15		; 50000 Nm,  i=156,   F-48, ����-45-1-�40,  45���, 1500 ��/���|
 .word 1		;1.  ������ ��� 0_0 +
 .word 27		;2.  ������ ��� 0_1 +
 .word 40		;3.  ������ ��� 0_2 +
 .word 60		;4.  ������ ��� 0_3 +
 .word 85		;5.  ������ ��� 0_4 +
 .word 6		;6.  ������ ��� 1_0 +
 .word 29		;7.  ������ ��� 1_1 +
 .word 52		;8.  ������ ��� 1_2 +
 .word 82		;9.  ������ ��� 1_3 +
 .word 105		;10. ������ ��� 1_4 +
 .word 1		;11. ������ ��� 2_0 +
 .word 32		;12. ������ ��� 2_1 +
 .word 56		;13. ������ ��� 2_2 +
 .word 84		;14. ������ ��� 2_3 +
 .word 110		;15. ������ ��� 2_4 +
 .word 10		;16. ������ ��� 3_0 +
 .word 40		;17. ������ ��� 3_1 +
 .word 62		;18. ������ ��� 3_2 +
 .word 95		;19. ������ ��� 3_3 +
 .word 120		;20. ������ ��� 3_4 +
 .word 70		;21. ������ ���� 0_0 +
 .word 30		;22. ������ ���� 0_1 +
 .word 10		;23. ������ ���� 0_2 +
 .word 3		;24. ������ ���� 0_3 +
 .word 1		;25. ������ ���� 0_4 +
 .word 90		;26. ������ ���� 1_0 +
 .word 52		;27. ������ ���� 1_1 +
 .word 22 		;28. ������ ���� 1_2 +
 .word 10		;29. ������ ���� 1_3 +
 .word 6		;30. ������ ���� 1_4 +
 .word 110		;31. ������ ���� 2_0 +
 .word 68		;32. ������ ���� 2_1 +
 .word 34		;33. ������ ���� 2_2 +
 .word 18		;34. ������ ���� 2_3 +
 .word 6		;35. ������ ���� 2_4 +
 .word 120		;36. ������ ���� 3_0 +
 .word 75		;37. ������ ���� 3_1 +
 .word 40		;38. ������ ���� 3_2 +
 .word 25		;39. ������ ���� 3_3 +
 .word 11		;40. ������ ���� 3_4 +
 .word 87		;41. ���� ���� 0_0 +
 .word 83		;42. ���� ���� 0_1 +
 .word 76		;43. ���� ���� 0_2 +
 .word 65		;44. ���� ���� 0_3 +
 .word 60		;45. ���� ���� 0_4 +
 .word 93		;46. ���� ���� 1_0 +
 .word 89		;47. ���� ���� 1_1 +
 .word 82		;48. ���� ���� 1_2 +
 .word 71		;49. ���� ���� 1_3 +
 .word 66		;50. ���� ���� 1_4 +
 .word 96		;51. ���� ���� 2_0 +
 .word 92		;52. ���� ���� 2_1 +
 .word 85		;53. ���� ���� 2_2 +
 .word 74		;54. ���� ���� 2_3 +
 .word 69		;55. ���� ���� 2_4 +
 .word 99		;56. ���� ���� 3_0 +
 .word 96		;57. ���� ���� 3_1 +
 .word 89		;58. ���� ���� 3_2 +
 .word 83		;59. ���� ���� 3_3 +
 .word 78		;60. ���� ���� 3_4 +
_drive16

_drive17	
_drive18	
_drive19	
_drive20	
