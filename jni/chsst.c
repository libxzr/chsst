#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/mount.h>
#include <sys/mount.h>

struct sst_option {
    const char *name;
    unsigned long flags;
};

static inline int change_mount(const char *mount_point, unsigned long flags)
{
    return mount(NULL, mount_point, NULL, flags, NULL);
}

static struct sst_option const sst_options[] = {
    { .name = "--make-shared", .flags = MS_SHARED },
    { .name = "--make-slave", .flags = MS_SLAVE },
    { .name = "--make-private", .flags = MS_PRIVATE },
    { .name = "--make-unbindable", .flags = MS_UNBINDABLE },
    { .name = "--make-rshared", .flags = MS_SHARED | MS_REC },
    { .name = "--make-rslave", .flags = MS_SLAVE | MS_REC },
    { .name = "--make-rprivate", .flags = MS_PRIVATE | MS_REC },
    { .name = "--make-runbindable", .flags = MS_UNBINDABLE | MS_REC }
};

static unsigned long get_flags_from_option(char *option)
{
    const struct sst_option *p = sst_options;
    int size = sizeof(sst_options) / sizeof(struct sst_option);

    do {
        if (!strcmp(p->name, option))
            return p->flags;
    } while (++p < sst_options + size);

    return 0;
}

static void help()
{
    printf("chsst (CHange Shared SubTrees) by LibXZR <i@xzr.moe>\n");
    printf("Usage:\n chsst <mountpoint> <operation>\n chsst <operation> <mountpoint>\n");
    printf("Operations:\n");
    printf(" --make-shared           mark a subtree as shared\n");
    printf(" --make-slave            mark a subtree as slave\n");
    printf(" --make-private          mark a subtree as private\n");
    printf(" --make-unbindable       mark a subtree as unbindable\n");
    printf(" --make-rshared          recursively mark a whole subtree as shared\n");
    printf(" --make-rslave           recursively mark a whole subtree as slave\n");
    printf(" --make-rprivate         recursively mark a whole subtree as private\n");
    printf(" --make-runbindable      recursively mark a whole subtree as unbindable\n");
}

int main(int argc, char *argv[])
{
    unsigned long flags = 0;
    char *mount_point = NULL;

    if (getuid() != 0) {
        printf("You must run this with root privilege\n");
        return 1;
    }

    // Skip filename
    argc--;
    argv++;

    if (argc == 0 || !strcmp(*argv, "--help")) {
        help();
        return 0;
    }

    while (argc--) {
        unsigned long new_flags;

        new_flags = get_flags_from_option(*argv);
        if (new_flags == 0) {
            if (mount_point != NULL) {
                printf("Invalid arguments. See --help for usage.\n");
                return 1;
            }
            // Treat this as mount point
            mount_point = *argv;
        } else {
            if (flags != 0) {
                printf("Redundant options. See --help for usage.\n");
                return 1;
            }
            flags = new_flags;
        }

        argv++;
    }

    if (mount_point == NULL || flags == 0) {
        printf("Missing arguments. See --help for usage.\n");
        return 1;
    }

    if (change_mount(mount_point, flags)) {
        printf("Error occured when trying to mount %s with flags = %lu\n", mount_point, flags);
        return 1;
    }

    return 0;
}
