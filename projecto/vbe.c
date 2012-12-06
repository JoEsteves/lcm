
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14
#define tamanho sizeof(vbe_mode_info_t)

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

  struct reg86u registos;
  mmap_t endereco;


  lm_init();
  lm_alloc(tamanho, &endereco);



  registos.u.w.ax = 0x4F01;
  registos.u.w.es = PB2BASE(endereco.phys);
  registos.u.w.di = PB2OFF(endereco.phys);
  registos.u.b.intno = 0x10;
  registos.u.w.cx = 1 << 14 | mode; //1x10^14 em bin



  sys_int86(&registos);

  vbe_mode_info_t *temp;
  temp = endereco.virtual;
  *vmi_p = *temp;

  lm_free(&endereco);

  return 0;
}
