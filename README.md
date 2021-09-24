# SHAPEIT4 fork with xSqueezeIt file format support

- This fork was made to add xSqueezeIt support to SHAPEIT4
- Explore what API would xSqueezeIt need to provide to be integrated
- Show a real world example of xSqueezeIt usage

```shell
# Compile with :
make XSI_SUPPORT=YES
# Run with the variant bcf file of xSqueezeIt (and not the binary file)
./bin/shapeit4.2 --input test/unphased.xsi_var.bcf --map test/chr20.b37.gmap.gz --region 20 --output xsi_phased2.bcf
```

It also supports xSqueezeIt for the reference / scaffolding. However, it does not support phase sets because xSqueezeIt does not support the `PS` field for the moment.

## xSqueezeIt

Github link : https://github.com/rwk-unil/xSqueezeIt

### Compress bcf files

```shell
# Compress the test file with xSqueezeIt (see build instructions in git link above)
xsqueezeit -c -f test/unphased.bcf -o test/unphased.xsi --zstd
# This will generate two files : unphased.xsi and unphased.xsi_var.bcf
# Both files are required, pass the bcf as input to SHAPEIT
...
xsqueezeit -c -f test/reference.bcf -o test/reference.xsi --zstd
...
```

# Segmented HAPlotype Estimation and Imputation Tools version 4 (SHAPEIT4)

SHAPEIT4 is a fast and accurate method for estimation of haplotypes (aka phasing) for SNP array and sequencing data. The version 4 is a refactored and improved version of the SHAPEIT algorithm with multiple key additional features:
- It includes a Positional Burrow Wheeler Transform (PBWT) based approach to quickly select a small set of informative conditioning haplotypes to be used when updating the phase of an individual.
- We have changed that way in which phase information in sequencing reads is input into the model. We now recommend the use of the WhatsHap tool as a pre-processing step to extract phase information from a bam file..
- It accounts for sets of pre-phased genotypes (i.e. haplotype scaffold). The scaffold can be derived either from family data or large reference panels.
- It reads and writes files using HTSlib for better I/O performance in either VCF or BCF formats.
- The genotype graph and HMM routines have been re-implemented for better hardware usage and performance.
- The source code is provided in an open source format (license MIT) on github.

If you use the SHAPEIT4 in your research work, please cite the following paper:

Delaneau O., et al. Accurate, scalable and integrative haplotype estimation. Nature Communications volume 10, Article number: 5436 (2019). 
https://www.nature.com/articles/s41467-019-13225-y

## Documentation

https://odelaneau.github.io/shapeit4/

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
