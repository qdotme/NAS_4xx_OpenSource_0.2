/* Automatically generated file. Do not edit. Edit wrapawk/wrapfunc.inp. */
#ifndef WRAPTMPF_H
#define WRAPTMPF_H
int tmp_open (const char *pathname, int flags, int mode){
  load_library_symbols();
  return  next_open (pathname, flags, mode);
}

FILE * tmp_fopen (const char *pathname, const char *mode){
  load_library_symbols();
  return  next_fopen (pathname, mode);
}

FILE * tmp_freopen (const char *pathname, const char *mode, FILE *stream){
  load_library_symbols();
  return  next_freopen (pathname, mode, stream);
}



int TMP_LSTAT LSTAT_ARG(int ver, const char *file_name, struct stat *buf){
  load_library_symbols();
  return  NEXT_LSTAT_NOARG LSTAT_ARG(ver, file_name, buf);
}

int TMP_STAT STAT_ARG(int ver, const char *file_name, struct stat *buf){
  load_library_symbols();
  return  NEXT_STAT_NOARG STAT_ARG(ver, file_name, buf);
}

int TMP_FSTAT FSTAT_ARG(int ver, int fd, struct stat *buf){
  load_library_symbols();
  return  NEXT_FSTAT_NOARG FSTAT_ARG(ver, fd, buf);
}


#ifdef STAT64_SUPPORT
int TMP_LSTAT64 LSTAT64_ARG(int ver, const char *file_name, struct stat64 *buf){
  load_library_symbols();
  return  NEXT_LSTAT64_NOARG LSTAT64_ARG(ver, file_name, buf);
}

int TMP_STAT64 STAT64_ARG(int ver, const char *file_name, struct stat64 *buf){
  load_library_symbols();
  return  NEXT_STAT64_NOARG STAT64_ARG(ver, file_name, buf);
}

int TMP_FSTAT64 FSTAT_ARG(int ver, int fd, struct stat64 *buf){
  load_library_symbols();
  return  NEXT_FSTAT64_NOARG FSTAT_ARG(ver, fd, buf);
}

#endif

int TMP_MKNOD MKNOD_ARG(int ver, const char *pathname, mode_t mode, dev_t XMKNOD_FRTH_ARG dev){
  load_library_symbols();
  return  NEXT_MKNOD_NOARG MKNOD_ARG(ver, pathname, mode, dev);
}



int tmp_chown (const char *path, uid_t owner, gid_t group){
  load_library_symbols();
  return  next_chown (path, owner, group);
}

int tmp_lchown (const char *path, uid_t owner, gid_t group){
  load_library_symbols();
  return  next_lchown (path, owner, group);
}

int tmp_fchown (int fd, uid_t owner, gid_t group){
  load_library_symbols();
  return  next_fchown (fd, owner, group);
}

int tmp_chmod (const char *path, mode_t mode){
  load_library_symbols();
  return  next_chmod (path, mode);
}

int tmp_fchmod (int fd, mode_t mode){
  load_library_symbols();
  return  next_fchmod (fd, mode);
}

int tmp_mkdir (const char *path, mode_t mode){
  load_library_symbols();
  return  next_mkdir (path, mode);
}

int tmp_unlink (const char *pathname){
  load_library_symbols();
  return  next_unlink (pathname);
}

int tmp_rmdir (const char *pathname){
  load_library_symbols();
  return  next_rmdir (pathname);
}

int tmp_remove (const char *pathname){
  load_library_symbols();
  return  next_remove (pathname);
}

int tmp_rename (const char *oldpath, const char *newpath){
  load_library_symbols();
  return  next_rename (oldpath, newpath);
}


#ifdef FAKEROOT_FAKENET
pid_t tmp_fork (void){
  load_library_symbols();
  return  next_fork ();
}

pid_t tmp_vfork (void){
  load_library_symbols();
  return  next_vfork ();
}

int tmp_close (int fd){
  load_library_symbols();
  return  next_close (fd);
}

int tmp_dup2 (int oldfd, int newfd){
  load_library_symbols();
  return  next_dup2 (oldfd, newfd);
}

#endif /* FAKEROOT_FAKENET */


uid_t tmp_getuid (void){
  load_library_symbols();
  return  next_getuid ();
}

gid_t tmp_getgid (void){
  load_library_symbols();
  return  next_getgid ();
}

uid_t tmp_geteuid (void){
  load_library_symbols();
  return  next_geteuid ();
}

gid_t tmp_getegid (void){
  load_library_symbols();
  return  next_getegid ();
}

int tmp_setuid (uid_t id){
  load_library_symbols();
  return  next_setuid (id);
}

int tmp_setgid (gid_t id){
  load_library_symbols();
  return  next_setgid (id);
}

int tmp_seteuid (uid_t id){
  load_library_symbols();
  return  next_seteuid (id);
}

int tmp_setegid (gid_t id){
  load_library_symbols();
  return  next_setegid (id);
}

int tmp_setreuid (SETREUID_ARG ruid, SETREUID_ARG euid){
  load_library_symbols();
  return  next_setreuid (ruid, euid);
}

int tmp_setregid (SETREGID_ARG rgid, SETREGID_ARG egid){
  load_library_symbols();
  return  next_setregid (rgid, egid);
}

#ifdef HAVE_GETRESUID
int tmp_getresuid (uid_t *ruid, uid_t *euid, uid_t *suid){
  load_library_symbols();
  return  next_getresuid (ruid, euid, suid);
}

#endif /* HAVE_GETRESUID */
#ifdef HAVE_GETRESGID
int tmp_getresgid (gid_t *rgid, gid_t *egid, gid_t *sgid){
  load_library_symbols();
  return  next_getresgid (rgid, egid, sgid);
}

#endif /* HAVE_GETRESGID */
#ifdef HAVE_SETRESUID
int tmp_setresuid (uid_t ruid, uid_t euid, uid_t suid){
  load_library_symbols();
  return  next_setresuid (ruid, euid, suid);
}

#endif /* HAVE_SETRESUID */
#ifdef HAVE_SETRESGID
int tmp_setresgid (gid_t rgid, gid_t egid, gid_t sgid){
  load_library_symbols();
  return  next_setresgid (rgid, egid, sgid);
}

#endif /* HAVE_SETRESGID */
#ifdef HAVE_SETFSUID
uid_t tmp_setfsuid (uid_t fsuid){
  load_library_symbols();
  return  next_setfsuid (fsuid);
}

#endif /* HAVE_SETFSUID */
#ifdef HAVE_SETFSGID
gid_t tmp_setfsgid (gid_t fsgid){
  load_library_symbols();
  return  next_setfsgid (fsgid);
}

#endif /* HAVE_SETFSGID */
int tmp_initgroups (const char *user, INITGROUPS_SECOND_ARG group){
  load_library_symbols();
  return  next_initgroups (user, group);
}

int tmp_setgroups (SETGROUPS_SIZE_TYPE size, const gid_t *list){
  load_library_symbols();
  return  next_setgroups (size, list);
}

#endif
