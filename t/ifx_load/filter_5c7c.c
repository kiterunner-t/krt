/**
 * Copyleft (C) KRT, 2011 2012 by kiterunner_t
 */
 
/*
 * GB18030�������: 
 *     ���ֽ� 0x00~0x7F
 *     ˫�ֽ� 0x81~0xFE  0x40~0xFE             (�ڶ��ֽڲ�����0x7F)
 *     ���ֽ� 0x81~0xFE  0x30~0x39  0x81~0xFE  0x30~0x39
 *
 * ���������ַ����ɱ���ת�����������, ����Ϊ�����ַ���ASCII��ֵ
 *    �ַ� ʮ������ ʮ����
 *     \   0x5C     92
 *     |   0x7C     124
 * #   !   0x21     33  (�Ƽ��ֶηָ���)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 8192

static void filter_5c7c(const char *);


int
main(int argc, char **argv)
{
  int i;
   
  if (argc < 2) { 
    printf("Usage: %s filename_list", argv[0]);
    return EXIT_FAILED;
  }

  for (i = 1; i < argc; i++)
    filter_5c7c(argv[i]);

  return EXIT_SUCCESS;
}


static void
filter_5c7c(const char *filename)
{
  FILE *fp;
  FILE *fpout;
  FILE *fperr;
  char  line[MAXLINES];
  char  out[MAXLINES];
  int   i;
  int   j;
  int   cnt;
  char  outfile[256];
  char  errfile[256];

  sprintf(outfile, "%s.tmp", filename);
  sprintf(errfile, "%s.err", filename);
  
  fp = fopen(filename, "r");
  fpout = fopen(outfile, "wb");
  fperr = fopen(errfile, "wb");
  if (fp==NULL || fpout==NULL || fperr==NULL) {
    printf("open file failed\n");
    exit(EXIT_FAILED);
  }

  for ( ; fgets(line, sizeof(line), fp) != NULL; ) {

    for (i = 0, j = 0; line[i] != '\0'; ) {
      if (line[i] == 0x21) {
          cnt++;
          fprintf(fperr, "%s\n", line);
          break;
      }

      /* ���������ַ� '\', ���ȡ��һ�� (����Ӧ��ѭ����ȡ, ���ܻ��ж��л���) */
      if (line[i]==0x5C && line[i+1]==0x0A) {
        if (fgets(line, sizeof(line), fp) == NULL)
          break;

        i = 0;
      }

      if (line[i] < 0x80) { /* ���ֽڱ��� */
          if (line[i] == 0x7C)  
              out[j] = '!';
          else 
              out[j] = line[i];

          i++;
          j++;

      } else if (line[i]>0x80 && line[i]<0xFF) {
        if (line[i+1]>0x2F && line[i+1]<0x3A) { /* ���ֽڱ��� */
          out[j] = line[i];
          out[j+1] = line[i+1];
          out[j+2] = line[i+2];
          out[j+3] = line[i+3];
          j += 4;
          i += 4;

        } else { /* ˫�ֽڱ��� */
          if (line[i+1] == 0x5C) {
            out[j] = line[i];
            out[j+1] = 0x5C;
            out[j+2] = 0x5C;
            i += 2;
            j += 3;

          } else {
            out[j] = line[i];
            out[j+1] = line[i+1];
            i += 2;
            j += 2;
          }
        }

      } else { /* ������������ */
        i++;
      }
    }

    out[j] = '\0';
    fprintf(fpout, "%s", out);
  }

  fclose(fp);
  fclose(fpout);
  fclose(fperr);

  if (!cnt)
    remove(errfile);
}

